
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
    
    utility::memoryCopy(&fileData[2], &fileHeader, 12);
    utility::memoryCopy(&fileData[14], &infoHeader, 40);
    
    SPADAS_ERROR_RETURN(infoHeader.infoHeaderSize != 40);
    SPADAS_ERROR_RETURN(infoHeader.compressType != 0);
    SPADAS_ERROR_RETURN(infoHeader.nPlanes != 1);

	Bool reverse = infoHeader.height > 0x7fffffff;
	if (reverse) infoHeader.height = (UInt)-infoHeader.height;
    
    UInt srcNChannels;
	Enum<PixelFormat> outputFormat;
    Bool usePalette;
    Binary r, g, b, a;
	switch (infoHeader.nBitsPerPixel)
	{
        case 8:
        {
            srcNChannels = 1;
            usePalette = TRUE;
            SPADAS_ERROR_RETURN(infoHeader.nColors != 0 && infoHeader.nColors != 256);
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
			SPADAS_ERROR_MSG("Invalid infoHeader.nBitsPerPixel");
            return;
	}
	
	Image out(Size2D::wh(infoHeader.width, infoHeader.height), outputFormat, FALSE);
    
	/* load image's data */
    Byte *srcData = &fileData[fileHeader.dataOffset];
    UInt srcStep = ((infoHeader.width * srcNChannels - 1) & 0xfffffffc) + 4;
    switch (outputFormat.value())
    {
        case PixelFormat::Value::ByteGray:
        {
            for (UInt v = 0; v < infoHeader.height; v++)
            {
                Byte *srcRow = &srcData[v * srcStep];
                Byte *dstRow = out[infoHeader.height - v - 1].b;
                for (UInt u = 0; u < infoHeader.width; u++)
                {
                    dstRow[u] = r[srcRow[u]];
                }
            }
        }
            break;
        case PixelFormat::Value::ByteBGR:
        {
            for (UInt v = 0; v < infoHeader.height; v++)
            {
                utility::memoryCopy(&srcData[v * srcStep], out[infoHeader.height - v - 1].ptr, infoHeader.width * 3);
            }
        }
            break;
        case PixelFormat::Value::ByteBGRA:
        {
            if (usePalette)
            {
                for (UInt v = 0; v < infoHeader.height; v++)
                {
                    Byte *srcRow = &srcData[v * srcStep];
                    Byte *dstRow = out[infoHeader.height - v - 1].b;
                    for (UInt u = 0; u < infoHeader.width; u++)
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
                for (UInt v = 0; v < infoHeader.height; v++)
                {
                    utility::memoryCopy(&srcData[v * srcStep], out[infoHeader.height - v - 1].ptr, infoHeader.width * 4);
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
    
	UInt dstStep = ((in.width() * PixelFormat::nChannels(outputFormat) - 1) & 0xfffffffc) + 4;
    
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
    
	/* copy image data to buffer */
	Binary fileData(fileHeader.fileSize);
    
	fileData[0] = 'B';
	fileData[1] = 'M';
    
    utility::memoryCopy(&fileHeader, &fileData[2], 12);
	utility::memoryCopy(&infoHeader, &fileData[14], 40);
    
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
            Byte *dstData = &fileData[54];
            for (UInt v = 0; v < infoHeader.height; v++)
            {
                Byte *dstRow = &dstData[v * dstStep];
                Byte *srcRow = in[infoHeader.height - v - 1].b;
                utility::memoryCopy(srcRow, dstRow, infoHeader.width * PixelFormat::nChannels(outputFormat));
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