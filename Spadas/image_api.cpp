
#include "spadas.h"

namespace image_internal
{
    using namespace spadas;

    struct DecodeImageContext
    {
        Image decoded;
    };

    struct EncodeImageContext
    {
        Binary encoded;
    };
}

using namespace image_internal;

extern "C"
{
	SPADAS_DEFAULT_API Pointer createDecodeImageContext(Byte *binaryPtr, Int binarySize, Int *meta/* width, height, with alpha(0/1) */)
	{
		if (!binaryPtr || binarySize == 0) return NULL;
        if (!meta) return NULL;

        Binary binary(binaryPtr, binarySize);
        if (binary.size() < 16) return NULL;

        Optional<Enum<ImageFileFormat> > format;
        for (UInt i = 0; i <= 8; i++)
        {
            if (binary[i] == 'B' && binary[i + 1] == 'M')
            {
                format = Enum<ImageFileFormat>(ImageFileFormat::Value::BMP);
                break;
            }
            if (binary[i] == 0xff && binary[i + 1] == 0xd8)
            {
                format = Enum<ImageFileFormat>(ImageFileFormat::Value::JPG);
                break;
            }
            if (binary[i] == 'P' && binary[i + 1] == 'N' && binary[i + 2] == 'G')
            {
                format = Enum<ImageFileFormat>(ImageFileFormat::Value::PNG);
                break;
            }
        }
        if (format.isNull()) return NULL;

        Image decoded(format.value(), binary);
        if (decoded.isNull()) return NULL;

        meta[0] = (Int)decoded.width();
        meta[1] = (Int)decoded.height();
        meta[2] = decoded.format() == PixelFormat::Value::ByteBGRA;

        DecodeImageContext *ctx = new DecodeImageContext();
        ctx->decoded = decoded;
        return ctx;
	}

	SPADAS_DEFAULT_API void copyDecodedImage(Pointer context, Byte* dstData, Int dstStep)
	{
        if (!context) return;

        DecodeImageContext *ctx = (DecodeImageContext*)context;

        if (ctx->decoded.format() == PixelFormat::Value::ByteGray)
        {
            const UInt width = ctx->decoded.width();
            for (UInt v = 0; v < ctx->decoded.height(); v++)
            {
                Byte *src = ctx->decoded[v].b;
                Byte *dst = dstData + v * dstStep;
                for (UInt u = 0; u < width; u++)
                {
                    Byte val = *src++;
                    *dst++ = val;
                    *dst++ = val;
                    *dst++ = val;
                }
            }
        }
        else
        {
            for (UInt v = 0; v < ctx->decoded.height(); v++)
            {
                utility::memoryCopy(ctx->decoded[v].ptr, dstData + v * dstStep, ctx->decoded.rowBytesValid());
            }
        }
	}

	SPADAS_DEFAULT_API void deleteDecodeImageContext(Pointer context)
	{
        if (context) delete (DecodeImageContext*)context;
	}

	SPADAS_DEFAULT_API Pointer createEncodeImageContext(Byte *imageDataPtr, Int type/* 1: jpg, 2: png */, Int imageWidth, Int imageHeight, Int rowBytes, Bool withAlpha, Int *encodedSize)
	{
		if (!imageDataPtr) return NULL;
        if (type <= 0 || type > 2) return NULL;
        if (imageWidth <= 0 || imageHeight <= 0 || rowBytes <= 0) return NULL;
        if (!encodedSize) return NULL;

        Image image(Size2D::wh(imageWidth, imageHeight), withAlpha ? PixelFormat::Value::ByteBGRA : PixelFormat::Value::ByteBGR, FALSE);
        for (Int v = 0; v < imageHeight; v++)
        {
            utility::memoryCopy(imageDataPtr + v * rowBytes, image[v].b, image.rowBytesValid());
        }

        Binary encoded = image.toBinary(type == 1 ? ImageFileFormat::Value::JPG : ImageFileFormat::Value::PNG, 80);
        if (encoded.isEmpty()) return NULL;

        *encodedSize = encoded.size();

        EncodeImageContext *ctx = new EncodeImageContext();
        ctx->encoded = encoded;
        return ctx;
	}

	SPADAS_DEFAULT_API void copyEncodedPacket(Pointer context, Byte* dstData)
	{
        if (!context) return;

        EncodeImageContext *ctx = (EncodeImageContext*)context;
        utility::memoryCopy(ctx->encoded.data(), dstData, ctx->encoded.size());
	}

	SPADAS_DEFAULT_API void deleteEncodeImageContext(Pointer context)
	{
        if (context) delete (EncodeImageContext*)context;
	}
}