
#if defined(SPADAS_ENV_MACOS)

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#include "image.h"

namespace image_internal
{
    using namespace spadas;

    Image decodeImage(Binary imageData)
    {
        SPADAS_ERROR_RETURNVAL(imageData.isEmpty(), Image());

        CFDataRef cfImageData = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, imageData.data(), imageData.size(), kCFAllocatorNull);
        CGImageSourceRef cgImageSource = CGImageSourceCreateWithData(cfImageData, NULL);
        CFRelease(cfImageData);
        SPADAS_ERROR_RETURNVAL(!cgImageSource, Image());

        UInt imageCount = (UInt)CGImageSourceGetCount(cgImageSource);
        if (imageCount != 1)
        {
            CFRelease(cgImageSource);
            SPADAS_ERROR_MSG("imageCount != 1");
            return Image();
        }
        
        CGImageRef cgImage = CGImageSourceCreateImageAtIndex(cgImageSource, 0, NULL);
        CFRelease(cgImageSource);
        SPADAS_ERROR_RETURNVAL(!cgImage, Image());

        UInt bitsPerComponent = (UInt)CGImageGetBitsPerComponent(cgImage);
        UInt bitsPerPixel = (UInt)CGImageGetBitsPerPixel(cgImage);
        CGBitmapInfo bitmapInfo = CGImageGetBitmapInfo(cgImage);
        if (bitsPerComponent != 8 || bitsPerPixel != 32 || (bitmapInfo != kCGImageAlphaLast && bitmapInfo != kCGImageAlphaNoneSkipLast))
        {
            CFRelease(cgImage);
            SPADAS_ERROR_MSG("bitsPerComponent != 8 || bitsPerPixel != 32 || (bitmapInfo != kCGImageAlphaLast && bitmapInfo != kCGImageAlphaNoneSkipLast)");
            return Image();
        }

        UInt imageWidth = (UInt)CGImageGetWidth(cgImage);
        UInt imageHeight = (UInt)CGImageGetHeight(cgImage);
        UInt bytesPerRow = (UInt)CGImageGetBytesPerRow(cgImage);

        CFDataRef cfDecodedData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        CFRelease(cgImage);
        SPADAS_ERROR_RETURNVAL(!cfDecodedData, Image());

        Image output(Size2D::wh(imageWidth, imageHeight), bitmapInfo == kCGImageAlphaLast ? PixelFormat::Value::ByteBGRA : PixelFormat::Value::ByteBGR, FALSE);
        const Byte *srcData = CFDataGetBytePtr(cfDecodedData);
        for (UInt v = 0; v < imageHeight; v++)
        {
            const UInt *srcRow = (const UInt*)(srcData + v * bytesPerRow);
            if (bitmapInfo == kCGImageAlphaLast)
            {
                UInt *dstRow = (UInt*)output[v].ptr;
                for (UInt u = 0; u < imageWidth; u++)
                {
                    UInt srcCell = srcRow[u];
                    dstRow[u] = ((srcCell & 0x00ff0000) >> 16) | ((srcCell & 0x000000ff) << 16) | (srcCell & 0xff00ff00);
                }
            }
            else
            {
                Byte *dstRow = output[v].b;
                for (UInt u = 0; u < imageWidth; u++)
                {
                    UInt srcCell = srcRow[u];
                    dstRow[3 * u] = (srcCell & 0x00ff0000) >> 16;
                    dstRow[3 * u + 1] = (srcCell & 0x0000ff00) >> 8;
                    dstRow[3 * u + 2] = srcCell & 0x000000ff;
                }
            }
        }
        CFRelease(cfDecodedData);
        return output;
    }
}

using namespace image_internal;

Image spadas::decodePNG(Binary pngData)
{
    return decodeImage(pngData);
}

Binary spadas::encodePNG(Image image)
{
    SPADAS_ERROR_RETURNVAL(image.isNull(), Binary());

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

    CGDataProviderRef imageDataProvider = CGDataProviderCreateWithData(NULL, image.data().ptr, image.height() * image.rowBytes(), NULL);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CGImageRef cgImage = CGImageCreate(image.width(), image.height(), 8, withAlpha ? 32 : 24, image.rowBytes(), colorSpace, withAlpha ? kCGImageAlphaLast : kCGImageAlphaNone, imageDataProvider, NULL, false, kCGRenderingIntentDefault);

    CFMutableDataRef cfOutput = CFDataCreateMutable(NULL, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(cfOutput, CFSTR("public.png"), 1, NULL);
    CGImageDestinationAddImage(destination, cgImage, NULL);
    CGImageDestinationFinalize(destination);

    const Byte *outputData = CFDataGetBytePtr(cfOutput);
    UInt outputSize = (UInt)CFDataGetLength(cfOutput);
    Binary output(outputData, outputSize);

    CFRelease(imageDataProvider);
    CFRelease(colorSpace);
    CFRelease(cgImage);    
    CFRelease(destination);
    CFRelease(cfOutput);

    return output;
}

Image spadas::decodeJPG(Binary jpgData)
{
    return decodeImage(jpgData);
}

Binary spadas::encodeJPG(Image image, UInt quality)
{
    SPADAS_ERROR_RETURNVAL(image.isNull(), Binary());

    if (image.format() != PixelFormat::Value::ByteRGB) image = image.convert(PixelFormat::Value::ByteRGB);

    CGDataProviderRef imageDataProvider = CGDataProviderCreateWithData(NULL, image.data().ptr, image.height() * image.rowBytes(), NULL);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
    CGImageRef cgImage = CGImageCreate(image.width(), image.height(), 8, 24, image.rowBytes(), colorSpace, kCGImageAlphaNone, imageDataProvider, NULL, false, kCGRenderingIntentDefault);

    CFMutableDataRef cfOutput = CFDataCreateMutable(NULL, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(cfOutput, CFSTR("public.jpeg"), 1, NULL);

    float compression = 0.01f * quality;

    CFStringRef propKeys[1] { kCGImageDestinationLossyCompressionQuality };
    CFTypeRef propValues[1] { CFNumberCreate(NULL, kCFNumberFloatType, &compression) };
    CFDictionaryRef options = CFDictionaryCreate(NULL, (const void **)propKeys, (const void **)propValues, 1,&kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CGImageDestinationAddImage(destination, cgImage, options);
    CGImageDestinationFinalize(destination);

    const Byte *outputData = CFDataGetBytePtr(cfOutput);
    UInt outputSize = (UInt)CFDataGetLength(cfOutput);
    Binary output(outputData, outputSize);

    CFRelease(propValues[0]);
    CFRelease(options);
    CFRelease(imageDataProvider);
    CFRelease(colorSpace);
    CFRelease(cgImage);    
    CFRelease(destination);
    CFRelease(cfOutput);

    return output;
}

#endif