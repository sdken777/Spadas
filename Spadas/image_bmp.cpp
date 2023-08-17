
#include "image.h"

namespace image_internal
{
	using namespace spadas;
    
	struct FileHeader
	{
		UInt fileSize;				// out: using
		Word reserved1;				// out: 0
		Word reserved2;				// out: 0
		UInt dataOffset;			// inout: using
	};
    
	struct InfoHeader
	{
		UInt infoHeaderSize;		// inout: 40
		UInt width;					// inout: using
		UInt height;				// inout: using
		Word nPlanes;				// inout: 1
		Word nBitsPerPixel;			// 8(Palette), 24(BGR), 32(BGRA)
		UInt compressType;			// inout: 0
		UInt dataSize;				// out: 0
		Int horizontalResolution;	// out: 2795
		Int verticalResolution;		// out: 2795
		UInt nColors;				// inout: 256
		UInt nImportantColors;		// out: 256
	};

    struct CIEXYZ
    {
        Int ciexyzX;
        Int ciexyzY;
        Int ciexyzZ;
    };

    struct CIEXYZTRIPLE
    {
        CIEXYZ ciexyzRed;
        CIEXYZ ciexyzGreen;
        CIEXYZ ciexyzBlue;
    };

    struct InfoHeaderV5
    {
        UInt bV5Size;               // inout: 124
        UInt bV5Width;              // inout: using
        UInt bV5Height;             // inout: using
        Word bV5Planes;             // inout: 1
        Word bV5BitCount;           // inout: 32(BGRA)
        UInt bV5Compression;        // inout: 3
        UInt bV5SizeImage;          // out: 0
        Int bV5XPelsPerMeter;       // out: 2795
        Int bV5YPelsPerMeter;       // out: 2795
        UInt bV5ClrUsed;            // out: 0
        UInt bV5ClrImportant;       // out: 0
        UInt bV5RedMask;            // inout: 0x00ff0000
        UInt bV5GreenMask;          // inout: 0x0000ff00
        UInt bV5BlueMask;           // inout: 0x000000ff
        UInt bV5AlphaMask;          // out: 0xff000000
        UInt bV5CSType;             // inout: 0x57696e20 "Win "
        CIEXYZTRIPLE bV5Endpoints;  // out: 0
        UInt bV5GammaRed;           // out: 0
        UInt bV5GammaGreen;         // out: 0
        UInt bV5GammaBlue;          // out: 0
        UInt bV5Intent;             // out: 0
        UInt bV5ProfileData;        // out: 0
        UInt bV5ProfileSize;        // out: 0
        UInt bV5Reserved;           // out: 0
    };
}

using namespace spadas;
using namespace image_internal;

Image::Image(Path bmpFilePath)
{
    SPADAS_ERROR_RETURN(bmpFilePath.isNull() || bmpFilePath.isFolder());

	/* check file extension */
    SPADAS_ERROR_RETURN(bmpFilePath.extension() != ".bmp");
    
	/* load data from file */
    SPADAS_ERROR_RETURN(!bmpFilePath.exist());
    
	File file = File::openBinary(bmpFilePath);
    if (file.isNull()) return;
    
	Binary fileData = file.input();
    
	/* check magic number */
    SPADAS_ERROR_RETURN(fileData[0] != 'B' || fileData[1] != 'M');
    
	/* load headers */
	FileHeader fileHeader;
	InfoHeader infoHeader;
    InfoHeaderV5 infoHeaderV5;
    
    utility::memoryCopy(&fileData[2], &fileHeader, 12);
    utility::memoryCopy(&fileData[14], &infoHeader, 40);

    UInt imageWidth = 0, imageHeight = 0, nColors = 0;
    Word nBitsPerPixel = 0;

    if (infoHeader.infoHeaderSize == 124)
    {
        utility::memoryCopy(&fileData[14], &infoHeaderV5, 124);

        SPADAS_ERROR_RETURN(infoHeaderV5.bV5Compression != 3);
        SPADAS_ERROR_RETURN(infoHeaderV5.bV5Planes != 1);
        SPADAS_ERROR_RETURN(infoHeaderV5.bV5RedMask != 0x00ff0000);
        SPADAS_ERROR_RETURN(infoHeaderV5.bV5GreenMask != 0x0000ff00);
        SPADAS_ERROR_RETURN(infoHeaderV5.bV5BlueMask != 0x000000ff);
        SPADAS_ERROR_RETURN(infoHeaderV5.bV5CSType != 0x57696e20);

        imageWidth = infoHeaderV5.bV5Width;
        imageHeight = infoHeaderV5.bV5Height;
        nColors = infoHeaderV5.bV5ClrUsed;
        nBitsPerPixel = infoHeaderV5.bV5BitCount;
    }
    else
    {
        SPADAS_ERROR_RETURN(infoHeader.infoHeaderSize != 40);
        SPADAS_ERROR_RETURN(infoHeader.compressType != 0);
        SPADAS_ERROR_RETURN(infoHeader.nPlanes != 1);

        imageWidth = infoHeader.width;
        imageHeight = infoHeader.height;
        nColors = infoHeader.nColors;
        nBitsPerPixel = infoHeader.nBitsPerPixel;
    }

	Bool reverse = imageHeight > 0x7fffffff;
	if (reverse) imageHeight = (UInt)-imageHeight;
    
    UInt srcNChannels;
	Enum<PixelFormat> outputFormat;
    Bool usePalette;
    Binary r, g, b, a;
	switch (nBitsPerPixel)
	{
        case 8:
        {
            srcNChannels = 1;
            usePalette = TRUE;
            SPADAS_ERROR_RETURN(nColors != 0 && nColors != 256);
            r = Binary(256);
            g = Binary(256);
            b = Binary(256);
            a = Binary(256);
            UInt rgbNotSameSum = 0, aNotZeroSum = 0, aNot255Sum = 0;
            for (UInt i = 0; i < 256; i++)
            {
                b[i] = fileData[54 + 4 * i + 0];
                g[i] = fileData[54 + 4 * i + 1];
                r[i] = fileData[54 + 4 * i + 2];
                a[i] = fileData[54 + 4 * i + 3];
                rgbNotSameSum += (r[i] != g[i] || r[i] != b[i]);
                aNotZeroSum += (a[i] != 0);
                aNot255Sum += (a[i] != 255);
            }
            if ((aNotZeroSum == 0 || aNot255Sum == 0) && rgbNotSameSum == 0) outputFormat = PixelFormat::Value::ByteGray;
            else outputFormat = PixelFormat::Value::ByteBGRA;
        }
            break;
        case 24:
            srcNChannels = 3;
            outputFormat = PixelFormat::Value::ByteBGR;
            usePalette = FALSE;
            break;
        case 32:
            srcNChannels = 4;
            outputFormat = PixelFormat::Value::ByteBGRA;
            usePalette = FALSE;
            break;
        default:
			SPADAS_ERROR_MSG("Invalid nBitsPerPixel");
            return;
	}
	
	Image out(Size2D::wh(imageWidth, imageHeight), outputFormat, FALSE);
    
	/* load image's data */
    Byte *srcData = &fileData[fileHeader.dataOffset];
    UInt srcStep = ((imageWidth * srcNChannels - 1) & 0xfffffffc) + 4;
    switch (outputFormat.value())
    {
        case PixelFormat::Value::ByteGray:
        {
            for (UInt v = 0; v < imageHeight; v++)
            {
                Byte *srcRow = &srcData[v * srcStep];
                Byte *dstRow = out[imageHeight - v - 1].b;
                for (UInt u = 0; u < imageWidth; u++)
                {
                    dstRow[u] = r[srcRow[u]];
                }
            }
        }
            break;
        case PixelFormat::Value::ByteBGR:
        {
            for (UInt v = 0; v < imageHeight; v++)
            {
                utility::memoryCopy(&srcData[v * srcStep], out[imageHeight - v - 1].ptr, imageWidth * 3);
            }
        }
            break;
        case PixelFormat::Value::ByteBGRA:
        {
            if (usePalette)
            {
                for (UInt v = 0; v < imageHeight; v++)
                {
                    Byte *srcRow = &srcData[v * srcStep];
                    Byte *dstRow = out[imageHeight - v - 1].b;
                    for (UInt u = 0; u < imageWidth; u++)
                    {
                        dstRow[4*u] = b[srcRow[u]];
                        dstRow[4*u+1] = g[srcRow[u]];
                        dstRow[4*u+2] = r[srcRow[u]];
                        dstRow[4*u+3] = a[srcRow[u]];
                    }
                }
            }
            else
            {
                for (UInt v = 0; v < imageHeight; v++)
                {
                    utility::memoryCopy(&srcData[v * srcStep], out[imageHeight - v - 1].ptr, imageWidth * 4);
                }
            }
        }
            break;
        default:
            break;
    }

	if (reverse) operator =(out.flip(TRUE, FALSE));
	else operator =(out);
}

void Image::save(Path bmpFilePath)
{
    SPADAS_ERROR_RETURN(!vars);
    SPADAS_ERROR_RETURN(bmpFilePath.isNull() || bmpFilePath.isFolder());

	/* check file extension */
    SPADAS_ERROR_RETURN(bmpFilePath.extension() != ".bmp");
    
	/* convert pixel format */
    Enum<PixelFormat> outputFormat;
    if (PixelFormat::hasAlpha(vars->format)) outputFormat = PixelFormat::Value::ByteBGRA;
    else if (PixelFormat::isColor(vars->format)) outputFormat = PixelFormat::Value::ByteBGR;
    else outputFormat = PixelFormat::Value::ByteGray;
    
	Image in(*this);
    if (vars->format != outputFormat) in = convert(outputFormat);
    
	/* generate headers */
	FileHeader fileHeader;
	InfoHeader infoHeader;
    InfoHeaderV5 infoHeaderV5;
    Bool useV5 = FALSE;
    
	UInt dstStep = ((in.width() * PixelFormat::nChannels(outputFormat) - 1) & 0xfffffffc) + 4;
    
    if (outputFormat == PixelFormat::Value::ByteGray || outputFormat == PixelFormat::Value::ByteBGR)
    {
        fileHeader.fileSize = 54 + (outputFormat == PixelFormat::Value::ByteGray ? 1024 : 0) + in.height() * dstStep;
        fileHeader.reserved1 = 0;
        fileHeader.reserved2 = 0;
        fileHeader.dataOffset = 54 + (outputFormat == PixelFormat::Value::ByteGray ? 1024 : 0);
        
        infoHeader.infoHeaderSize = 40;
        infoHeader.width = in.width();
        infoHeader.height = in.height();
        infoHeader.nPlanes = 1;
        infoHeader.nBitsPerPixel = PixelFormat::nChannels(outputFormat) * 8;
        infoHeader.compressType = 0;
        infoHeader.dataSize = 0;
        infoHeader.horizontalResolution = 2795;
        infoHeader.verticalResolution = 2795;
        infoHeader.nColors = (outputFormat == PixelFormat::Value::ByteGray ? 256 : 0);
        infoHeader.nImportantColors = (outputFormat == PixelFormat::Value::ByteGray ? 256 : 0);
    }
    else // PixelFormat::ByteBGRA
    {
        fileHeader.fileSize = 138 + in.height() * dstStep;
        fileHeader.reserved1 = 0;
        fileHeader.reserved2 = 0;
        fileHeader.dataOffset = 138;

        infoHeaderV5.bV5Size = 124;
        infoHeaderV5.bV5Width = in.width();
        infoHeaderV5.bV5Height = in.height();
        infoHeaderV5.bV5Planes = 1;
        infoHeaderV5.bV5BitCount = 32;
        infoHeaderV5.bV5Compression = 3;
        infoHeaderV5.bV5SizeImage = 0;
        infoHeaderV5.bV5XPelsPerMeter = 2795;
        infoHeaderV5.bV5YPelsPerMeter = 2795;
        infoHeaderV5.bV5ClrUsed = 0;
        infoHeaderV5.bV5ClrImportant = 0;
        infoHeaderV5.bV5RedMask = 0x00ff0000;
        infoHeaderV5.bV5GreenMask = 0x0000ff00;
        infoHeaderV5.bV5BlueMask = 0x000000ff;
        infoHeaderV5.bV5AlphaMask = 0xff000000;
        infoHeaderV5.bV5CSType = 0x57696e20;
        infoHeaderV5.bV5Endpoints.ciexyzRed.ciexyzX = 0;
        infoHeaderV5.bV5Endpoints.ciexyzRed.ciexyzY = 0;
        infoHeaderV5.bV5Endpoints.ciexyzRed.ciexyzZ = 0;
        infoHeaderV5.bV5Endpoints.ciexyzGreen.ciexyzX = 0;
        infoHeaderV5.bV5Endpoints.ciexyzGreen.ciexyzY = 0;
        infoHeaderV5.bV5Endpoints.ciexyzGreen.ciexyzZ = 0;
        infoHeaderV5.bV5Endpoints.ciexyzBlue.ciexyzX = 0;
        infoHeaderV5.bV5Endpoints.ciexyzBlue.ciexyzY = 0;
        infoHeaderV5.bV5Endpoints.ciexyzBlue.ciexyzZ = 0;
        infoHeaderV5.bV5GammaRed = 0;
        infoHeaderV5.bV5GammaGreen = 0;
        infoHeaderV5.bV5GammaBlue = 0;
        infoHeaderV5.bV5Intent = 0;
        infoHeaderV5.bV5ProfileData = 0;
        infoHeaderV5.bV5ProfileSize = 0;
        infoHeaderV5.bV5Reserved = 0;

        useV5 = TRUE;
    }
    
	/* copy image data to buffer */
	Binary fileData(fileHeader.fileSize);
    
	fileData[0] = 'B';
	fileData[1] = 'M';
    
    utility::memoryCopy(&fileHeader, &fileData[2], 12);
    if (useV5) utility::memoryCopy(&infoHeaderV5, &fileData[14], 124);
    else utility::memoryCopy(&infoHeader, &fileData[14], 40);
    
    switch (outputFormat.value())
    {
        case PixelFormat::Value::ByteGray:
        {
            Byte *dstPaletteData = &fileData[54];
            Byte *dstPixelData = &fileData[54 + 1024];
            for (UInt i = 0; i < 256; i++)
            {
                dstPaletteData[4*i] = dstPaletteData[4*i+1] = dstPaletteData[4*i+2] = (Byte)i;
                dstPaletteData[4*i+3] = 255;
            }
            for (UInt v = 0; v < infoHeader.height; v++)
            {
                Byte *dstRow = &dstPixelData[v * dstStep];
                Byte *srcRow = in[infoHeader.height - v - 1].b;
                utility::memoryCopy(srcRow, dstRow, infoHeader.width);
            }
        }
            break;
        case PixelFormat::Value::ByteBGR:
        case PixelFormat::Value::ByteBGRA:
        {
            Byte *dstData = &fileData[fileHeader.dataOffset];
            for (UInt v = 0; v < in.height(); v++)
            {
                Byte *dstRow = &dstData[v * dstStep];
                Byte *srcRow = in[in.height() - v - 1].b;
                utility::memoryCopy(srcRow, dstRow, in.width() * PixelFormat::nChannels(outputFormat));
            }
        }
            break;
        default:
            break;
    }
    
	/* write buffer to file */
	File file = File::createBinary(bmpFilePath);
	if (!file.isValid()) return;
    
	file.output(fileData);
}