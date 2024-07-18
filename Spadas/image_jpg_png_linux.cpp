
#if defined(SPADAS_ENV_LINUX)

#include <stdio.h>
#include "jpeglib.h"
#include "png.h"
#include "image.h"

namespace image_internal
{
    using namespace spadas;

    struct ReadPNGContext
    {
        Byte *pngDataPtr;
        UInt pngDataSize;
        UInt curOffset;
    };

    struct WritePNGContext
    {
        List<Binary> buffer;
    };

    void readPNGData(png_structp png_ptr, png_byte* raw_data, png_size_t read_length)
    {
        ReadPNGContext *readContext = (ReadPNGContext*)png_get_io_ptr(png_ptr);
        UInt copyCount = math::min(readContext->pngDataSize - readContext->curOffset, (UInt)read_length);
        utility::memoryCopy(readContext->pngDataPtr + readContext->curOffset, raw_data, copyCount);
        readContext->curOffset += copyCount;
    }

    void writePNGData(png_structp png_ptr, png_byte* raw_data, png_size_t write_length)
    {
        WritePNGContext *writeContext = (WritePNGContext*)png_get_io_ptr(png_ptr);
        Binary packet(raw_data, write_length);
        writeContext->buffer.addToTail(packet);
    }

    struct DecodeJPGErrorContainer
    {
        jpeg_error_mgr error;
        jmp_buf jumpBuffer;
    };

    void decodeJPGErrorExit(j_common_ptr context)
    {
        DecodeJPGErrorContainer *container = (DecodeJPGErrorContainer*)context->err;
        (*context->err->output_message)(context);
        longjmp(container->jumpBuffer, 1);
    }
};

using namespace image_internal;

Image spadas::decodePNG(Binary pngData)
{
    SPADAS_ERROR_RETURNVAL(pngData.isEmpty(), Image());

    png_structp context = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    SPADAS_ERROR_RETURNVAL(!context, Image());

    png_infop info = png_create_info_struct(context);
    if (!info)
    {
        png_destroy_read_struct(&context, NULL, NULL);
        SPADAS_ERROR_MSG("!info");
        return Image();
    }

    if (setjmp(png_jmpbuf(context)))
    {
        png_destroy_info_struct(context, &info);
        png_destroy_read_struct(&context, NULL, NULL);
        SPADAS_ERROR_MSG("Decoder failed");
        return Image();
    }

    ReadPNGContext readContext;
    readContext.pngDataPtr = pngData.data();
    readContext.pngDataSize = pngData.size();
    readContext.curOffset = 0;
    png_set_read_fn(context, &readContext, readPNGData);

    png_read_info(context, info);
    UInt width = png_get_image_width(context, info);
    UInt height = png_get_image_height(context, info);
    png_byte colorType = png_get_color_type(context, info);
    png_byte bitDepth = png_get_bit_depth(context, info);

    if (bitDepth == 16) png_set_strip_16(context);
    if (colorType == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(context);
    if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) png_set_expand_gray_1_2_4_to_8(context);
    if (png_get_valid(context, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(context);
    if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE) png_set_filler(context, 0xFF, PNG_FILLER_AFTER);
    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(context);
    png_read_update_info(context, info);
    UInt rowBytes = png_get_rowbytes(context, info);
    
    Bool withAlpha = FALSE;
    if (rowBytes == width * 4) withAlpha = TRUE;
    else if (rowBytes < width * 3)
    {
        png_destroy_info_struct(context, &info);
        png_destroy_read_struct(&context, NULL, NULL);
        SPADAS_ERROR_MSG("rowBytes < width * 3");
        return Image();
    }

    Binary buffer(height * rowBytes);
    Array<Byte*> rowPtrs(height);
    for (UInt v = 0; v < height; v++)
    {
        rowPtrs[v] = &buffer[v * rowBytes];
    }
    png_read_image(context, rowPtrs.data());

    Image output(Size2D::wh(width, height), withAlpha ? PixelFormat::Value::ByteBGRA : PixelFormat::Value::ByteBGR, FALSE);
    for (UInt v = 0; v < height; v++)
    {
        if (withAlpha)
        {
            UInt *src = (UInt*)rowPtrs[v];
            UInt *dst = (UInt*)output[v].ptr;
            for (UInt u = 0; u < width; u++)
            {
                UInt srcVal = src[u];
                dst[u] = ((srcVal & 0x000000ff) << 16) | ((srcVal & 0x00ff0000) >> 16) | (srcVal & 0xff00ff00);
            }
        }
        else
        {
            Byte *src = rowPtrs[v];
            Byte *dst = output[v].b;
            for (UInt u = 0; u < width; u++)
            {
                dst[3 * u] = src[3 * u + 2];
                dst[3 * u + 1] = src[3 * u + 1];
                dst[3 * u + 2] = src[3 * u];
            }
        }
    }

    png_destroy_info_struct(context, &info);
    png_destroy_read_struct(&context, NULL, NULL);
    return output;
}

Binary spadas::encodePNG(Image image)
{
    SPADAS_ERROR_RETURNVAL(image.isNull(), Binary());

    png_structp context = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    SPADAS_ERROR_RETURNVAL(!context, Binary());

    png_infop info = png_create_info_struct(context);
    if (!info)
    {
        png_destroy_write_struct(&context, NULL);
        SPADAS_ERROR_MSG("!info");
        return Binary();
    }

    if (setjmp(png_jmpbuf(context)))
    {
        png_destroy_info_struct(context, &info);
        png_destroy_write_struct(&context, NULL);
        SPADAS_ERROR_MSG("setjmp(png_jmpbuf(context))");
        return Binary();
    }

    Bool withAlpha = FALSE;
    if (PixelFormat::hasAlpha(image.format()))
    {
        withAlpha = TRUE;
        if (image.format() != PixelFormat::Value::ByteRGBA) image = image.convert(PixelFormat::Value::ByteRGBA);
    }
    else
    {
        if (image.format() != PixelFormat::Value::ByteRGB) image = image.convert(PixelFormat::Value::ByteRGB);
    }

    png_set_IHDR(context, info, image.width(), image.height(), 8, withAlpha ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    Array<Byte*> rowPointers(image.height());
    for (UInt v = 0; v < image.height(); v++) rowPointers[v] = image[v].b;

    WritePNGContext writeContext;
    png_set_write_fn(context, &writeContext, writePNGData, NULL);
    png_set_rows(context, info, rowPointers.data());
    png_write_png(context, info, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&context, &info);

    return Binary::merge(writeContext.buffer.toArray());
}

Image spadas::decodeJPG(Binary jpgData)
{
    SPADAS_ERROR_RETURNVAL(jpgData.isEmpty(), Image());

    jpeg_decompress_struct context;
    DecodeJPGErrorContainer errorContainer;
    context.err = jpeg_std_error(&errorContainer.error);
    errorContainer.error.error_exit = decodeJPGErrorExit;
    if (setjmp(errorContainer.jumpBuffer))
    {
        jpeg_destroy_decompress(&context);
        SPADAS_ERROR_MSG("Decoder failed");
        return Image();
    }

    jpeg_create_decompress(&context);
    jpeg_mem_src(&context, jpgData.data(), jpgData.size());

    int result = jpeg_read_header(&context, TRUE);
    if (result != 1)
    {
        jpeg_destroy_decompress(&context);
        SPADAS_ERROR_MSG("jpeg_read_header failed");
        return Image();
    }

    Bool ok = jpeg_start_decompress(&context);
    if (!ok)
    {
        jpeg_destroy_decompress(&context);
        SPADAS_ERROR_MSG("jpeg_start_decompress failed");
        return Image();
    }

    UInt width = context.output_width;
    UInt height = context.output_height;
    UInt components = context.output_components;
    if (width == 0 || height == 0 || components != 3)
    {
        jpeg_finish_decompress(&context);
        jpeg_destroy_decompress(&context);
        SPADAS_ERROR_MSG("width == 0 || height == 0 || components != 3");
        return Image();
    }

    Image output(Size2D::wh(width, height), PixelFormat::Value::ByteBGR, FALSE);
    Binary buffer(width * 3);
    for (UInt v = 0; v < height; v++)
    {
        Byte* target[1] { buffer.data() };
        jpeg_read_scanlines(&context, target, 1);

        Byte *srcRow = buffer.data();
        Byte *dstRow = output[v].b;
        for (UInt u = 0; u < width; u++)
        {
            dstRow[3 * u] = srcRow[3 * u + 2];
            dstRow[3 * u + 1] = srcRow[3 * u + 1];
            dstRow[3 * u + 2] = srcRow[3 * u];
        }
    }

    jpeg_finish_decompress(&context);
    jpeg_destroy_decompress(&context);
    return output;
}

Binary spadas::encodeJPG(Image image, UInt quality)
{
    SPADAS_ERROR_RETURNVAL(image.isNull(), Binary());
    
    if (image.format() != PixelFormat::Value::ByteRGB) image = image.convert(PixelFormat::Value::ByteRGB);

    jpeg_compress_struct context;
    jpeg_error_mgr errorMsg;
    context.err = jpeg_std_error(&errorMsg);
    jpeg_create_compress(&context);

    context.image_width = image.width();
    context.image_height = image.height();
    context.input_components = 3;
    context.in_color_space = JCS_RGB;
    jpeg_set_defaults(&context);
    jpeg_set_quality(&context, quality, TRUE);

    Byte *outputData = NULL;
    unsigned long outputDataSize = 0;
    jpeg_mem_dest(&context, &outputData, &outputDataSize);

    jpeg_start_compress(&context, TRUE);

    JSAMPROW rowPointer[1];
    for (UInt v = 0; v < image.height(); v++)
    {
        rowPointer[0] = image[v].b;
        jpeg_write_scanlines(&context, rowPointer, 1);
    }
    
    jpeg_finish_compress(&context);
    jpeg_destroy_compress(&context);

    Binary output(outputData, outputDataSize);
    free(outputData);
    return output;
}

#endif