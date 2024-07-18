
#if defined(SPADAS_ENV_WINDOWS)

#include <wincodec.h>
#include "image.h"

namespace image_internal
{
    using namespace spadas;

    IWICImagingFactory *imagingFactory = NULL;
    Bool ensureImagingFactory()
    {
        if(imagingFactory == NULL)
        {
            CoInitializeEx(NULL, COINIT_MULTITHREADED);
            CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, IID_PPV_ARGS_Helper(&imagingFactory));
        }
        return imagingFactory != NULL;
    }

    Image decodeImage(Binary imageData, Bool withAlpha)
    {
        SPADAS_ERROR_RETURNVAL(imageData.isEmpty(), Image());
        SPADAS_ERROR_RETURNVAL(!ensureImagingFactory(), Image());

        IWICStream *stream = NULL;
        imagingFactory->CreateStream(&stream);
        SPADAS_ERROR_RETURNVAL(!stream, Image());

        stream->InitializeFromMemory(imageData.data(), imageData.size());

        IWICBitmapDecoder *decoder = NULL;
        imagingFactory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnDemand, &decoder);
        if (!decoder)
        {
            stream->Release();
            SPADAS_ERROR_MSG("CreateDecoderFromStream failed");
            return Image();
        }

        IWICBitmapFrameDecode *frameDecode = NULL;
        decoder->GetFrame(0, &frameDecode);
        if (!frameDecode)
        {
            decoder->Release();
            stream->Release();
            SPADAS_ERROR_MSG("GetFrame failed");
            return Image();
        }

        IWICFormatConverter *converter = NULL;
        imagingFactory->CreateFormatConverter(&converter);
        if (!converter)
        {
            frameDecode->Release();
            decoder->Release();
            stream->Release();
            SPADAS_ERROR_MSG("CreateFormatConverter failed");
            return Image();
        }

        converter->Initialize(frameDecode, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);

        IWICBitmap *bitmap = NULL;
        imagingFactory->CreateBitmapFromSource(converter, WICBitmapCacheOnDemand, &bitmap);
        if (!bitmap)
        {
            converter->Release();
            frameDecode->Release();
            decoder->Release();
            stream->Release();
            SPADAS_ERROR_MSG("CreateBitmapFromSource failed");
            return Image();
        }

        UInt width = 0, height = 0;
        bitmap->GetSize(&width, &height);

        IWICBitmapLock *lock = NULL;
        WICRect rect = { 0, 0, (Int)width, (Int)height };
        bitmap->Lock(&rect, WICBitmapLockRead, &lock);

        UInt dataSize = 0;
        Byte *data = NULL;
        lock->GetDataPointer(&dataSize, &data);

        Image output(Size2D::wh(width, height), withAlpha ? PixelFormat::Value::ByteBGRA : PixelFormat::Value::ByteBGR, FALSE);
        UInt srcRowBytes = dataSize / height;
        for (UInt v = 0; v < height; v++)
        {
            Byte *srcRow = data + v * srcRowBytes;
            Byte *dstRow = output[v].b;
            if (withAlpha)
            {
                utility::memoryCopy(srcRow, dstRow, width * 4);
            }
            else
            {
                for (UInt u = 0; u < width; u++)
                {
                    dstRow[3 * u] = srcRow[4 * u];
                    dstRow[3 * u + 1] = srcRow[4 * u + 1];
                    dstRow[3 * u + 2] = srcRow[4 * u + 2];
                }
            }
        }

        lock->Release();
        bitmap->Release();
        converter->Release();
        frameDecode->Release();
        decoder->Release();
        stream->Release();
        return output;
    }

    Binary encodeImage(Image image, UInt jpegQuality = UINF)
    {
        SPADAS_ERROR_RETURNVAL(image.isNull(), Binary());
        SPADAS_ERROR_RETURNVAL(!ensureImagingFactory(), Binary());

        IWICStream *stream = NULL;
        imagingFactory->CreateStream(&stream);
        SPADAS_ERROR_RETURNVAL(!stream, Binary());

        IStream *buffer = NULL;
        CreateStreamOnHGlobal(NULL, TRUE, &buffer);
        if (!buffer)
        {
            stream->Release();
            SPADAS_ERROR_MSG("CreateStreamOnHGlobal failed");
            return Binary();
        }
        stream->InitializeFromIStream(buffer);

        IWICBitmapEncoder *encoder = NULL;
        imagingFactory->CreateEncoder(jpegQuality == UINF ? GUID_ContainerFormatPng : GUID_ContainerFormatJpeg, NULL, &encoder);
        if (!encoder)
        {
            stream->Release();
            buffer->Release();
            SPADAS_ERROR_MSG("CreateEncoder failed");
            return Binary();
        }
        encoder->Initialize(stream, WICBitmapEncoderNoCache);

        IWICBitmapFrameEncode *frame = NULL;
        IPropertyBag2 *properties = NULL;
        encoder->CreateNewFrame(&frame, &properties);
        if (!frame || !properties)
        {
            encoder->Release();
            stream->Release();
            buffer->Release();
            SPADAS_ERROR_MSG("CreateNewFrame failed");
            return Binary();
        }

        if (jpegQuality == UINF && PixelFormat::hasAlpha(image.format()))
        {
            if (image.format() != PixelFormat::Value::ByteBGRA) image = image.convert(PixelFormat::Value::ByteBGRA);
        }
        else
        {
            if (image.format() != PixelFormat::Value::ByteBGR) image = image.convert(PixelFormat::Value::ByteBGR);
        }

        if (jpegQuality != UINF)
        {
            WChar qualityString[] = L"ImageQuality";
            PROPBAG2 options = { 0 };
            options.pstrName = qualityString;
            options.dwType = PROPBAG2_TYPE_DATA;
            options.vt = VT_R4;
            VARIANT valValue;
            VariantInit(&valValue);
            valValue.vt = VT_R4;
            valValue.fltVal = 0.01f * jpegQuality;
            properties->Write(1, &options, &valValue);
        }
        frame->Initialize(properties);

        frame->SetSize(image.width(), image.height());

        WICPixelFormatGUID format = image.format() == PixelFormat::Value::ByteBGRA ? GUID_WICPixelFormat32bppBGRA : GUID_WICPixelFormat24bppBGR;
        frame->SetPixelFormat(&format);

        frame->WritePixels(image.height(), image.rowBytes(), image.height() * image.rowBytes(), image.data().b);
        frame->Commit();
        encoder->Commit();

        STATSTG stat;
        stream->Stat(&stat, 0);

        LARGE_INTEGER seekPos;
        seekPos.QuadPart = 0;
        buffer->Seek(seekPos, STREAM_SEEK_SET, NULL);

        Binary output((UInt)stat.cbSize.QuadPart);
        buffer->Read(output.data(), output.size(), NULL);

        frame->Release();
        properties->Release();
        encoder->Release();
        stream->Release();
        buffer->Release();
        return output;
    }
}

using namespace image_internal;

Image spadas::decodePNG(Binary pngData)
{
    return decodeImage(pngData, TRUE);
}

Binary spadas::encodePNG(Image image)
{
    return encodeImage(image);
}

Image spadas::decodeJPG(Binary jpgData)
{
    return decodeImage(jpgData, FALSE);
}

Binary spadas::encodeJPG(Image image, UInt quality)
{
    return encodeImage(image, quality);
}

#endif