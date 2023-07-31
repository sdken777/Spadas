
#include "spadas.h"

#include "image_bz.h"

namespace image_internal
{
	using namespace spadas;
    
    ColorConvertTable table;
    
    UInt calcImageDataSize(Size2D size, Enum<PixelFormat> format)
    {
        size.width = math::clamp(size.width, 1u, MAX_WIDTH);
        size.height = math::clamp(size.height, 1u, MAX_HEIGHT);
        UInt rowBytesValid = size.width * PixelFormat::bytesPerPixel(format);
        return size.height * (((rowBytesValid - 1) >> 3) + 1);
    }
}

using namespace spadas;
using namespace spadas::math;
using namespace spadas::console;
using namespace spadas::utility;
using namespace image_internal;

const String spadas::Image::TypeName = "spadas.Image";

Image::Image()
{
}

Image::Image(Size2D size, Enum<PixelFormat> format, Bool clearPixels)
{
	setVars(new ImageVars(calcImageDataSize(size, format)), TRUE);
    size.width = math::clamp(size.width, 1u, MAX_WIDTH);
    size.height = math::clamp(size.height, 1u, MAX_HEIGHT);
    vars->width = size.width;
    vars->height = size.height;
    vars->format = format;
    vars->rowBytes = ((size.width * PixelFormat::bytesPerPixel(format) - 1) & 0xfffffff8) + 8;
    vars->bytesPerPixel = PixelFormat::bytesPerPixel(vars->format);
    vars->data = vars->data0.data();
    if (clearPixels)
    {
        utility::memorySet(0, vars->data, vars->rowBytes * vars->height);
    }
}

Image::Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Bool clearPixels)
{
    operator =(Image(ImageResolution::size(resolution), format, clearPixels));
}

Image::Image(Size2D size, Enum<PixelFormat> format, Pointer raw, UInt rawRowBytes)
{
	SPADAS_ERROR_RETURN(!raw);

	setVars(new ImageVars(calcImageDataSize(size, format)), TRUE);
    size.width = math::clamp(size.width, 1u, MAX_WIDTH);
    size.height = math::clamp(size.height, 1u, MAX_HEIGHT);
    vars->width = size.width;
    vars->height = size.height;
    vars->format = format;
    vars->rowBytes = ((size.width * PixelFormat::bytesPerPixel(format) - 1) & 0xfffffff8) + 8;
    vars->bytesPerPixel = PixelFormat::bytesPerPixel(vars->format);
    vars->data = vars->data0.data();
    Byte *srcData = (Byte*)raw;
    Byte *dstData = (Byte*)vars->data;
    UInt rowBytesValid = vars->width * PixelFormat::bytesPerPixel(format);
    for (UInt v = 0; v < vars->height; v++)
    {
        utility::memoryCopy(&srcData[v * rawRowBytes], &dstData[v * vars->rowBytes], rowBytesValid);
    }
}

Image::Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Pointer raw, UInt rawRowBytes)
{
    operator =(Image(ImageResolution::size(resolution), format, raw, rawRowBytes));
}

Image::Image(Image src, Region2D srcRegion)
{
    srcRegion.width = math::clamp(srcRegion.width, 1u, MAX_WIDTH);
    srcRegion.height = math::clamp(srcRegion.height, 1u, MAX_HEIGHT);
	SPADAS_ERROR_RETURN(srcRegion.offsetU < 0 || srcRegion.offsetV < 0 || srcRegion.offsetU + srcRegion.width > src.width() || srcRegion.offsetV + srcRegion.height > src.height());

	setVars(new ImageVars(calcImageDataSize(srcRegion.size(), src.format())), TRUE);
	vars->width = srcRegion.width;
	vars->height = srcRegion.height;
	vars->format = src.format();
	vars->rowBytes = ((vars->width * PixelFormat::bytesPerPixel(vars->format) - 1) & 0xfffffff8) + 8;
	vars->bytesPerPixel = PixelFormat::bytesPerPixel(vars->format);
	vars->data = vars->data0.data();

	Byte *srcData = src.data().b;
	UInt srcRowBytes = src.rowBytes();
	Byte *dstData = (Byte*)vars->data;
	UInt bytesPerPixel = PixelFormat::bytesPerPixel(vars->format);
	UInt rowBytesValid = vars->width * bytesPerPixel;
	for (UInt v = 0; v < vars->height; v++)
	{
		Byte *srcRow = &srcData[(srcRegion.offsetV + v) * srcRowBytes + srcRegion.offsetU * bytesPerPixel];
		Byte *dstRow = &dstData[v * vars->rowBytes];
		utility::memoryCopy(srcRow, dstRow, rowBytesValid);
	}
}

Image::Image(ImagePointer pointer)
{
    SPADAS_ERROR_RETURN(pointer.width == 0);
    SPADAS_ERROR_RETURN(pointer.height == 0);
    SPADAS_ERROR_RETURN(pointer.rowBytes < pointer.width * (pointer.isColor ? 3 : 1));
    SPADAS_ERROR_RETURN(pointer.data.size() * sizeof(ULong) != pointer.height * pointer.rowBytes);

    setVars(new ImageVars(pointer.data), TRUE);

	vars->width = pointer.width;
	vars->height = pointer.height;
	vars->format = pointer.isColor ? PixelFormat::ByteBGR : PixelFormat::ByteGray;
	vars->rowBytes = pointer.rowBytes;
	vars->bytesPerPixel = PixelFormat::bytesPerPixel(vars->format);
	vars->data = vars->data0.data();
}

Image::Image(IplImage *iplimage)
{
	SPADAS_ERROR_RETURN(!iplimage);
	
	Bool isSupported;
	Enum<PixelFormat> format;
	switch (iplimage->depth)
	{
        case 8:
            switch (iplimage->nChannels)
		{
            case 1:
                format = PixelFormat::ByteGray;
                isSupported = TRUE;
                break;
            case 3:
                format = PixelFormat::ByteBGR;
                isSupported = TRUE;
                break;
            case 4:
                format = PixelFormat::ByteBGRA;
                isSupported = TRUE;
                break;
            default:
                isSupported = FALSE;
                break;
		}
            break;
            
        case 16:
            switch (iplimage->nChannels)
		{
            case 1:
                format = PixelFormat::WordGray;
                isSupported = TRUE;
                break;
            case 3:
                format = PixelFormat::WordBGR;
                isSupported = TRUE;
                break;
            default:
                isSupported = FALSE;
                break;
		}
            break;
            
        case 32:
            switch (iplimage->nChannels)
		{
            case 1:
                format = PixelFormat::FloatGray;
                isSupported = TRUE;
                break;
            case 3:
                format = PixelFormat::FloatBGR;
                isSupported = TRUE;
                break;
            default:
                isSupported = FALSE;
                break;
		}
            break;
            
        default:
            isSupported = FALSE;
            break;
	}
    
	SPADAS_ERROR_RETURN(!isSupported);
    
    Size2D size = Size2D::wh(iplimage->width, iplimage->height);
	Image out(size, format, FALSE);
    
    UInt srcRowBytes = iplimage->widthStep;
    UInt rowBytesValid = out.rowBytesValid();
	for (UInt v = 0; v < size.height; v++)
	{
        utility::memoryCopy(&iplimage->imageData[v * srcRowBytes], out[v].b, rowBytesValid);
	}
	operator =(out);
}

Image Image::clone()
{
	SPADAS_ERROR_RETURNVAL(!vars, Image());
    Size2D size = Size2D::wh(vars->width, vars->height);
    CoordInt2D offset = CoordInt2D::uv(0, 0);
    Image out(size, vars->format, FALSE);
    out.copy(*this, Region2D(offset, size), offset);
    return out;
}

void Image::copy(Image src, Region2D srcRegion, CoordInt2D thisOffset)
{
	SPADAS_ERROR_RETURN(!vars);
	SPADAS_ERROR_RETURN(!src.vars || vars == src.vars);
	SPADAS_ERROR_RETURN(vars->format != src.format());
    
    srcRegion.width = math::clamp(srcRegion.width, 1u, MAX_WIDTH);
    srcRegion.height = math::clamp(srcRegion.height, 1u, MAX_HEIGHT);
	SPADAS_ERROR_RETURN(srcRegion.offsetU < 0 || srcRegion.offsetV < 0 || srcRegion.offsetU + srcRegion.width > src.width() || srcRegion.offsetV + srcRegion.height > src.height());
	SPADAS_ERROR_RETURN(thisOffset.u < 0 || thisOffset.v < 0 || thisOffset.u + srcRegion.width > vars->width || thisOffset.v + srcRegion.height > vars->height);

    Byte *srcData = src.data().b;
    UInt srcRowBytes = src.rowBytes();
    Byte *dstData = (Byte*)vars->data;
    UInt bytesPerPixel = PixelFormat::bytesPerPixel(vars->format);
    UInt rowBytesValid = srcRegion.width * bytesPerPixel;
    for (UInt vSrc = srcRegion.offsetV, vDst = thisOffset.v; vSrc < srcRegion.offsetV + srcRegion.height; vSrc++, vDst++)
    {
        Byte *srcRow = &srcData[vSrc * srcRowBytes + srcRegion.offsetU * bytesPerPixel];
        Byte *dstRow = &dstData[vDst * vars->rowBytes + thisOffset.u * bytesPerPixel];
        utility::memoryCopy(srcRow, dstRow, rowBytesValid);
    }
}

void Image::drawLayer(Image layer, CoordInt2D offset)
{
	SPADAS_ERROR_RETURN(!vars);
	SPADAS_ERROR_RETURN(!layer.vars);

    Enum<PixelFormat> srcFormat;
    switch (layer.format().value())
    {
        case PixelFormat::ByteBGRA:
            srcFormat = PixelFormat::ByteBGR;
            break;
        case PixelFormat::ByteRGBA:
            srcFormat = PixelFormat::ByteRGB;
            break;
        default:
			SPADAS_ERROR_MSG("Invalid layer.format()");
            return;
    }
    
	SPADAS_ERROR_RETURN(vars->format != srcFormat);
    
    Size2D layerSize = layer.size();
    if (offset.u >= (Int)vars->width || offset.v >= (Int)vars->height || offset.u + (Int)layerSize.width <= 0 || offset.v + (Int)layerSize.height <= 0) return;
    
    UInt uBegin = (UInt)math::max(0, offset.u);
    UInt vBegin = (UInt)math::max(0, offset.v);
    UInt uEnd = (UInt)math::min((Int)vars->width-1, offset.u + (Int)layerSize.width - 1);
    UInt vEnd = (UInt)math::min((Int)vars->height-1, offset.v + (Int)layerSize.height - 1);
    
    for (UInt v = vBegin; v <= vEnd; v++)
    {
        PixelData thisRow = operator[](v);
        PixelData layerRow = layer[(Int)v - offset.v];
        for (UInt u = uBegin; u <= uEnd; u++)
        {
            Byte *thisCell = thisRow[u].b;
            Byte *layerCell = layerRow[(Int)u - offset.u].b;
			
            UInt thisC0 = thisCell[0], thisC1 = thisCell[1], thisC2 = thisCell[2];
            UInt layerC0 = layerCell[0], layerC1 = layerCell[1], layerC2 = layerCell[2], alpha = layerCell[3];
            thisCell[0] = (thisC0 * (255 - alpha) + layerC0 * alpha + 128) >> 8;
            thisCell[1] = (thisC1 * (255 - alpha) + layerC1 * alpha + 128) >> 8;
            thisCell[2] = (thisC2 * (255 - alpha) + layerC2 * alpha + 128) >> 8;
        }
    }
}

Image Image::subImage(Region2D region)
{
	SPADAS_ERROR_RETURNVAL(!vars, Image());

	SPADAS_ERROR_RETURNVAL(vars->format == PixelFormat::ByteUYVY && (region.offsetU % 2 == 1 || region.width % 2 == 1), Image());
    
    region.width = math::clamp(region.width, 1u, MAX_WIDTH);
    region.height = math::clamp(region.height, 1u, MAX_HEIGHT);
	SPADAS_ERROR_RETURNVAL(region.offsetU < 0 || region.offsetV < 0 || region.offsetU + region.width > vars->width || region.offsetV + region.height > vars->height, Image(region.size(), vars->format));
	
    ImageVars *newVars = new ImageVars(vars->data0);
    newVars->width = region.width;
    newVars->height = region.height;
    newVars->format = vars->format;
    newVars->rowBytes = vars->rowBytes;
    newVars->bytesPerPixel = PixelFormat::bytesPerPixel(newVars->format);
    newVars->data = (Pointer)((PointerInt)vars->data + region.offsetV * vars->rowBytes + region.offsetU * PixelFormat::bytesPerPixel(vars->format));
	
	Image out;
	out.setVars(newVars, TRUE);
    return out;
}

Optional<ImagePointer> Image::imagePointer()
{
	SPADAS_ERROR_RETURNVAL(!vars, Optional<ImagePointer>());

	if ((PointerInt)vars->data != (PointerInt)vars->data0.data()) return Optional<ImagePointer>();

	if (vars->format == PixelFormat::ByteBGR)
	{
		ImagePointer ip;
		ip.width = vars->width;
		ip.height = vars->height;
		ip.isColor = TRUE;
		ip.rowBytes = vars->rowBytes;
		ip.data = vars->data0;
		return ip;
	}
	else if (vars->format == PixelFormat::ByteGray)
	{
		ImagePointer ip;
		ip.width = vars->width;
		ip.height = vars->height;
		ip.isColor = FALSE;
		ip.rowBytes = vars->rowBytes;
		ip.data = vars->data0;
		return ip;
	}
	else return Optional<ImagePointer>();
}

IplImage *Image::iplImage()
{
	SPADAS_ERROR_RETURNVAL(!vars, NULL);

	if (!vars->iplImage) vars->iplImage = new IplImage();
    
    if (vars->format != PixelFormat::ByteBGR &&
        vars->format != PixelFormat::ByteGray &&
        vars->format != PixelFormat::WordBGR &&
        vars->format != PixelFormat::WordGray &&
        vars->format != PixelFormat::FloatBGR &&
        vars->format != PixelFormat::FloatGray &&
        vars->format != PixelFormat::ByteBGRA)
    {
		SPADAS_ERROR_MSG("Invalid vars->format");
        return NULL;
    }
	
	Bool isColor = PixelFormat::isColor(vars->format);
    Bool hasAlpha = PixelFormat::hasAlpha(vars->format);
    vars->iplImage->nChannels = PixelFormat::nChannels(vars->format);
    vars->iplImage->depth = PixelFormat::bytesPerPixel(vars->format) * 8 / vars->iplImage->nChannels;
    
	if (isColor)
	{
        vars->iplImage->colorModel[0] = 'R';
		vars->iplImage->colorModel[1] = 'G';
		vars->iplImage->colorModel[2] = 'B';
        vars->iplImage->colorModel[3] = '\0';
        vars->iplImage->channelSeq[0] = 'B';
		vars->iplImage->channelSeq[1] = 'G';
		vars->iplImage->channelSeq[2] = 'R';
        if (hasAlpha) vars->iplImage->channelSeq[3] = 'A';
        else vars->iplImage->channelSeq[3] = '\0';
	}
	else
	{
		vars->iplImage->colorModel[0] = 'G';
		vars->iplImage->colorModel[1] = 'R'; 
		vars->iplImage->colorModel[2] = 'A';
		vars->iplImage->colorModel[3] = 'Y';
		vars->iplImage->channelSeq[0] = 'G';
		vars->iplImage->channelSeq[1] = 'R';
		vars->iplImage->channelSeq[2] = 'A';
		vars->iplImage->channelSeq[3] = 'Y';
	}
    
	vars->iplImage->nSize = sizeof(IplImage);
	vars->iplImage->ID = 0;
	vars->iplImage->alphaChannel = 0;
	vars->iplImage->dataOrder = 0;
	vars->iplImage->origin = 0;
	vars->iplImage->align = 8;
	vars->iplImage->width = (int)vars->width;
	vars->iplImage->height = (int)vars->height;
	vars->iplImage->roi = 0;
	vars->iplImage->maskROI = 0;
	vars->iplImage->imageId = 0;
	vars->iplImage->tileInfo = 0;
	vars->iplImage->imageSize = (int)(vars->rowBytes * vars->height);
	vars->iplImage->imageData = (char*)vars->data;
	vars->iplImage->widthStep = (int)vars->rowBytes;
	vars->iplImage->imageDataOrigin = (char*)vars->data0.data();
    
	for (int i = 0; i < 4; i++)
	{
		vars->iplImage->BorderMode[i] = 0;
		vars->iplImage->BorderConst[i] = 0;
	}
    
	return vars->iplImage;
}

Size2D Image::size()
{
	SPADAS_ERROR_RETURNVAL(!vars, Size2D());
	return Size2D::wh(vars->width, vars->height);
}

UInt Image::width()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return vars->width;
}

UInt Image::height()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return vars->height;
}

Enum<PixelFormat> Image::format()
{
    SPADAS_ERROR_RETURNVAL(!vars, PixelFormat::defaultValue());
	return vars->format;
}

UInt Image::rowBytes()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return vars->rowBytes;
}

UInt Image::rowBytesValid()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return vars->width * PixelFormat::bytesPerPixel(vars->format);
}

PixelData Image::data()
{
    SPADAS_ERROR_RETURNVAL(!vars, PixelData());
	return PixelData(vars->data, vars->bytesPerPixel);
}

PixelData Image::operator[](UInt v)
{
    SPADAS_ERROR_RETURNVAL(!vars, PixelData());
    SPADAS_ERROR_RETURNVAL(v >= vars->height, PixelData());
	return PixelData((Pointer)((PointerInt)vars->data + v * vars->rowBytes), vars->bytesPerPixel);
}

Image Image::resize(Enum<ResizeScale> resizeScale)
{
    SPADAS_ERROR_RETURNVAL(!vars, Image());

	if (vars->format != PixelFormat::ByteBGR &&
        vars->format != PixelFormat::ByteGray &&
        vars->format != PixelFormat::WordBGR &&
        vars->format != PixelFormat::WordGray &&
        vars->format != PixelFormat::ByteRGB &&
        vars->format != PixelFormat::ByteYUV &&
        vars->format != PixelFormat::ByteBGRA &&
        vars->format != PixelFormat::ByteRGBA)
	{
		SPADAS_ERROR_MSG("Invalid vars->format");
		return Image();
	}
	
	Int scale = 1;
	Float scaleF = 1.0f;
	switch (resizeScale.value())
	{
		case ResizeScale::Quarter:
			scale = 4;
			scaleF = 4.0f;
			break;
		case ResizeScale::Half:
			scale = 2;
			scaleF = 2.0f;
			break;
		case ResizeScale::Double:
			scale = 1;
			scaleF = 0.5f;
			break;
	}
	
    SPADAS_ERROR_RETURNVAL(vars->width % scale != 0 || vars->height % scale != 0, Image());
	
	Size2D targetSize = Size2D::wh(round((Float)vars->width / scaleF), round((Float)vars->height / scaleF));
	
    Image image(targetSize, vars->format, FALSE);
	switch (vars->format.value())
	{
		case PixelFormat::ByteBGR:
		case PixelFormat::ByteRGB:
		case PixelFormat::ByteYUV:
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
            resizeColorByteWithScale(*this, image, resizeScale, PixelFormat::nChannels(vars->format));
			break;
        case PixelFormat::ByteGray:
			resizeGrayByteWithScale(*this, image, resizeScale);
			break;
		case PixelFormat::WordBGR:
			resizeColorWordWithScale(*this, image, resizeScale);
			break;
		case PixelFormat::WordGray:
			resizeGrayWordWithScale(*this, image, resizeScale);
			break;

		default:
			break;
	}
    return image;
}

/* only for Nearest and Bilinear modes */
void resizeNearestOrBilinear(Image src, Image& dst, Enum<ResizeMode> mode)
{
    SPADAS_ERROR_RETURN(src.format() != dst.format());
	
	RowIOType iotype = getRowIOType(src.format());
	IRowIO *rowLoader = createRowLoader(src.format());
	IRowIO *rowWriter = createRowWriter(src.format());
	IRowRsz* rowRsz = createRowRsz(iotype);
	
	Float v = 0;
	Float scaleHeight = 0;
	if (dst.height() == 1)
	{
		scaleHeight = 0;
	}
	else
	{
		scaleHeight = (Float)(src.height() - 1) / (dst.height() - 1);
	}
	
	Int rowToRead = 0;
	if (mode == ResizeMode::Nearest)
	{
		rowToRead = 1;
	}
	if (mode == ResizeMode::Bilinear)
	{
		rowToRead = 2;
	}
	Pointer* dataIn = 0;
	dataIn = new Pointer[rowToRead];
	for (Int i = 0; i < rowToRead; i++)
	{
		dataIn[i] = createRowData(iotype, src.width());
	}
	Pointer dataOut = createRowData(iotype, dst.width());
	
	if (mode == ResizeMode::Nearest)
	{
		UInt prev = 99999;
		for (UInt y = 0; y < dst.height(); y++)
		{
			v = y * scaleHeight;
			v = (Float)round(v);
			if (v >= (Float)src.height() - 1)
			{
				v = (Float)src.height() - 1;
			}
			if (prev != v)
			{
				rowLoader->io(src[(Int)v].ptr, dataIn[0], src.width());
			}
			prev = (UInt)round(v);
			rowRsz->rsz(dataIn, dataOut, src.width(), dst.width(), src.height(), dst.height(), mode, v);
			rowWriter->io(dst[y].ptr, dataOut, dst.width());
		}
	}
	if(mode == ResizeMode::Bilinear)
	{
		UInt prefloorv = 99999;
		for (UInt y = 0; y < dst.height(); y ++)
		{
			v = y * scaleHeight;
			UInt floorv = floor(v);
			UInt ceilv = floorv + 1;
			if(ceilv > src.height() - 1)
			{
				ceilv = floorv;
			}
			if (prefloorv != floorv)
			{
				rowLoader->io(src[floorv].ptr, dataIn[0], src.width());
				rowLoader->io(src[ceilv].ptr, dataIn[1], src.width());
			}
			prefloorv = floorv;
			
			rowRsz->rsz(dataIn, dataOut, src.width(), dst.width(), src.height(), dst.height(), mode, v);
			rowWriter->io(dst[y].ptr, dataOut, dst.width());
		}
	}
	
	for (Int i = 0; i < rowToRead; i++)
	{
		destroyRowData(dataIn[i], iotype);
	}
	delete[] dataIn;
	destroyRowData(dataOut, iotype);
	delete rowLoader;
	delete rowWriter;
	delete rowRsz;
}

Image Image::resize(Enum<ImageResolution> outputResolution, Bool undistort, Enum<ResizeMode> mode)
{
    return resize(ImageResolution::size(outputResolution), undistort, mode);
}

Image Image::resize(Size2D outputSize, Bool undistort, Enum<ResizeMode> mode)
{
    SPADAS_ERROR_RETURNVAL(!vars, Image());
    SPADAS_ERROR_RETURNVAL(outputSize.width == 0 || outputSize.height == 0, Image());

	if (size() == outputSize) return clone();

	if (mode == ResizeMode::Nearest || mode == ResizeMode::Bilinear)
	{
        table.init();
		
        Image image(outputSize, vars->format, FALSE);
		Image sub(*this);
		if (undistort)
		{
			UInt srcWidth = vars->width;
			UInt srcHeight = vars->height;
			UInt dstWidth = outputSize.width;
			UInt dstHeight = outputSize.height;
			
			if (srcWidth * dstHeight > dstWidth * srcHeight) // clip Left-Right
			{
				UInt srcWidth0 = max(1u, dstWidth * srcHeight / dstHeight);
                UInt srcStartCol = (srcWidth - srcWidth0) / 2;
                sub = subImage(Region2D(CoordInt2D::uv(srcStartCol, 0), Size2D::wh(srcWidth0, srcHeight)));
			}
			else  // clip Up-Down
			{
				UInt srcHeight0 = max(1u, srcWidth * dstHeight / dstWidth);
                UInt srcStartRow = (srcHeight - srcHeight0) / 2;
                sub = subImage(Region2D(CoordInt2D::uv(0, srcStartRow), Size2D::wh(srcWidth, srcHeight0)));
			}
		}
		resizeNearestOrBilinear(sub, image, mode);
        return image;
	}
	
	else // RM_Area
	{
        if (vars->format != PixelFormat::ByteBGR &&
            vars->format != PixelFormat::ByteGray &&
            vars->format != PixelFormat::WordBGR &&
            vars->format != PixelFormat::WordGray &&
            vars->format != PixelFormat::FloatBGR &&
            vars->format != PixelFormat::FloatGray &&
            vars->format != PixelFormat::ByteRGB &&
            vars->format != PixelFormat::ByteYUV &&
            vars->format != PixelFormat::ByteBGRA &&
            vars->format != PixelFormat::ByteRGBA)
        {
			SPADAS_ERROR_MSG("Invalid vars->format");
            return Image();
        }
            
		Float widthRatio = (Float)vars->width / outputSize.width;
		Float heightRatio = (Float)vars->height / outputSize.height;
		
		if (undistort)
		{
			widthRatio = math::min(widthRatio, heightRatio);
			heightRatio = widthRatio;
		}
		
        Image image(outputSize, vars->format, FALSE);
		switch (vars->format.value())
		{
			case PixelFormat::ByteBGR:
            case PixelFormat::ByteRGB:
			case PixelFormat::ByteYUV:
            case PixelFormat::ByteBGRA:
            case PixelFormat::ByteRGBA:
                areaResizeColorByte(*this, image, widthRatio, heightRatio, PixelFormat::nChannels(vars->format));
				break;
			case PixelFormat::ByteGray:
				areaResizeGrayByte(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::WordBGR:
				areaResizeColorWord(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::WordGray:
				areaResizeGrayWord(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::FloatBGR:
				areaResizeColorFloat(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::FloatGray:
				areaResizeGrayFloat(*this, image, widthRatio, heightRatio);
				break;
			default:
				break;
		}
        return image;
	}
}

Image Image::convert(Enum<PixelFormat> outputFormat)
{
    SPADAS_ERROR_RETURNVAL(!vars, Image());

	if (outputFormat == vars->format) return clone();

	table.init();
	
    Image image(Size2D::wh(vars->width, vars->height), outputFormat, FALSE);

    Bool fastConverted = TRUE;
    switch (vars->format.value())
    {
        case PixelFormat::ByteBGR:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteGray:
                fastCvtByteBGR2ByteGray(*this, image);
                break;
            case PixelFormat::FloatBGR:
                fastCvtByteBGR2FloatBGR(*this, image);
                break;
            case PixelFormat::FloatGray:
                fastCvtByteBGR2FloatGray(*this, image);
                break;
            case PixelFormat::WordBGR:
                fastCvtByteBGR2WordBGR(*this, image);
                break;
            case PixelFormat::WordGray:
                fastCvtByteBGR2WordGray(*this, image);
                break;
            case PixelFormat::ByteRGB:
                fastCvtByteBGR2ByteRGB(*this, image);
                break;
            case PixelFormat::ByteUYVY:
                fastCvtByteBGR2ByteUYVY(*this, image);
                break;
            case PixelFormat::ByteYUV:
                fastCvtByteBGR2ByteYUV(*this, image);
                break;
            case PixelFormat::ByteBGRA:
                fastCvtByteBGR2ByteBGRA(*this, image);
                break;
            case PixelFormat::ByteRGBA:
                fastCvtByteBGR2ByteRGBA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::ByteGray:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtByteGray2ByteBGR(*this, image);
                break;
            case PixelFormat::FloatGray:
                fastCvtByteGray2FloatGray(*this, image);
                break;
            case PixelFormat::WordGray:
                fastCvtByteGray2WordGray(*this, image);
                break;
            case PixelFormat::ByteUYVY:
                fastCvtByteGray2ByteUYVY(*this, image);
                break;
            case PixelFormat::ByteYUV:
                fastCvtByteGray2ByteYUV(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::FloatBGR:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtFloatBGR2ByteBGR(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::FloatGray:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtFloatGray2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteGray:
                fastCvtFloatGray2ByteGray(*this, image);
                break;
            case PixelFormat::WordGray:
                fastCvtFloatGray2WordGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::WordBGR:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtWordBGR2ByteBGR(*this, image);
                break;
            case PixelFormat::WordGray:
                fastCvtWordBGR2WordGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::WordGray:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtWordGray2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteGray:
                fastCvtWordGray2ByteGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::ByteRGB:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtByteRGB2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteRGBA:
                fastCvtByteRGB2ByteRGBA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::ByteUYVY:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtByteUYVY2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteGray:
                fastCvtByteUYVY2ByteGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::ByteYUV:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtByteYUV2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteGray:
                fastCvtByteYUV2ByteGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::ByteBGRA:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtByteBGRA2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteRGBA:
                fastCvtByteBGRA2ByteRGBA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::ByteRGBA:
            switch (outputFormat.value())
        {
            case PixelFormat::ByteBGR:
                fastCvtByteRGBA2ByteBGR(*this, image);
                break;
            case PixelFormat::ByteRGB:
                fastCvtByteRGBA2ByteRGB(*this, image);
                break;
            case PixelFormat::ByteBGRA:
                fastCvtByteRGBA2ByteBGRA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        default:
            fastConverted = FALSE;
            break;
    }
	
	if (!fastConverted)
	{
        RowIOType srcType = getRowIOType(vars->format);
        RowIOType dstType = getRowIOType(outputFormat);
        
        IRowIO *rowLoader = createRowLoader(vars->format);
        IRowIO *rowWriter = createRowWriter(outputFormat);
        IRowCvt *rowCvt = dstType == srcType ? NULL : createRowCvt(srcType, dstType);
        
        Pointer dataIn = createRowData(srcType, vars->width);
        Pointer dataOut = dstType == srcType ? dataIn : createRowData(dstType, vars->width);
        
        for (UInt v = 0; v < vars->height; v++)
        {
            rowLoader->io(operator[](v).ptr, dataIn, vars->width);
            if (dstType != srcType) rowCvt->cvt(dataIn, dataOut, vars->width);
            rowWriter->io(image[v].ptr, dataOut, vars->width);
        }
        
        destroyRowData(dataIn, srcType);
        if (dstType != srcType) destroyRowData(dataOut, dstType);
        
        delete rowLoader;
        delete rowWriter;
        delete rowCvt;
	}
    
    return image;
}

Image Image::flip(Bool upDownFlip, Bool leftRightFlip)
{
    SPADAS_ERROR_RETURNVAL(!vars, Image());

    if (leftRightFlip && upDownFlip && vars->format != PixelFormat::ByteUYVY) return rotate(ImageRotation::CW180);
	
	table.init();
	
    Image image(Size2D::wh(vars->width, vars->height), vars->format, FALSE);
	
	if (leftRightFlip)
	{
        RowIOType type = getRowIOType(vars->format);
        
        IRowIO *rowLoader = createRowLoader(vars->format);
        IRowIO *rowWriter = createRowWriter(vars->format);
        IRowCvt *rowFlip = createRowFlip(type);
        
        Pointer dataIn = createRowData(type, vars->width);
        Pointer dataOut = createRowData(type, vars->width);
        
        if (upDownFlip)
        {
            for (UInt vSrc = 0, vDst = vars->height - 1; vSrc < vars->height; vSrc++, vDst--)
            {
                rowLoader->io(operator[](vSrc).ptr, dataIn, vars->width);
                rowFlip->cvt(dataIn, dataOut, vars->width);
                rowWriter->io(image[vDst].ptr, dataOut, vars->width);
            }
        }
        else
        {
            for (UInt v = 0; v < vars->height; v++)
            {
                rowLoader->io(operator[](v).ptr, dataIn, vars->width);
                rowFlip->cvt(dataIn, dataOut, vars->width);
                rowWriter->io(image[v].ptr, dataOut, vars->width);
            }
        }
        
        destroyRowData(dataIn, type);
        destroyRowData(dataOut, type);
        
        delete rowLoader;
        delete rowWriter;
        delete rowFlip;
	}
	else	/* !leftRightFlip */
	{
		UInt rowBytes = min(rowBytesValid(), image.rowBytesValid());
		if (upDownFlip)
		{
			for (UInt vSrc = 0, vDst = vars->height - 1; vSrc < vars->height; vSrc++, vDst--)
			{
				memoryCopy(operator[](vSrc).ptr, image[vDst].ptr, rowBytes);
			}
		}
		else
		{
			for (UInt v = 0; v < vars->height; v++)
			{
				memoryCopy(operator[](v).ptr, image[v].ptr, rowBytes);
			}
		}
	}
    
    return image;
}

Image Image::rotate(Enum<ImageRotation> rotation)
{
    SPADAS_ERROR_RETURNVAL(!vars, Image());
    SPADAS_ERROR_RETURNVAL(vars->format == PixelFormat::ByteUYVY, Image());

	Size2D targetSize;
	if (rotation == ImageRotation::CW180) targetSize = this->size();
	else targetSize = Size2D::wh(vars->height, vars->width);
    
    Image image(targetSize, vars->format, FALSE);
	
	switch (vars->format.value())
	{
		case PixelFormat::ByteBGR:
		case PixelFormat::ByteGray:
        case PixelFormat::ByteRGB:
		case PixelFormat::ByteYUV:
        case PixelFormat::ByteBool:
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
			rotateByte(*this, image, PixelFormat::nChannels(vars->format), rotation);
			break;
		case PixelFormat::WordBGR:
		case PixelFormat::WordGray:
			rotateWord(*this, image, PixelFormat::isColor(vars->format), rotation);
			break;
		case PixelFormat::FloatBGR:
		case PixelFormat::FloatGray:
		case PixelFormat::FloatHSV:
			rotateInt(*this, image, PixelFormat::isColor(vars->format), rotation);
			break;
		default:
			break;
	}
    return image;
}

ImagePair Image::splitFields()
{
    SPADAS_ERROR_RETURNVAL(!vars, ImagePair());

	Image dst[2];
	switch(vars->format.value())
	{
	case PixelFormat::ByteGray:
		splitFieldByte(*this, dst, 1);
		break;
	case PixelFormat::ByteUYVY:
		splitFieldByte(*this, dst, 2);
		break;
	case PixelFormat::ByteBGR:
	case PixelFormat::ByteRGB:
	case PixelFormat::ByteYUV:
		splitFieldByte(*this, dst, 3);
		break;
	case PixelFormat::ByteBGRA:
	case PixelFormat::ByteRGBA:
		splitFieldByte(*this, dst, 4);
		break;
	case PixelFormat::WordGray:
		splitFieldWord(*this, dst, 1);
		break;
	case PixelFormat::WordBGR:
		splitFieldWord(*this, dst, 3);
		break;
	case PixelFormat::FloatGray:
		splitFieldFloat(*this, dst, 1);
		break;
	case PixelFormat::FloatBGR:
		splitFieldFloat(*this, dst, 3);
		break;
	default:
		SPADAS_ERROR_MSG("Invalid vars->format");
		return ImagePair();
	}
    return ImagePair(dst[0], dst[1]);
}

Image Image::mergeFields(ImagePair fields)
{
	if (!fields.image1.vars && !fields.image2.vars) return Image();

	if (!fields.image1.vars) fields.image1 = Image(fields.image2.size(), fields.image2.format());
	if (!fields.image2.vars) fields.image2 = Image(fields.image1.size(), fields.image1.format());

	Image srcArr[2] = {fields.image1, fields.image2};
	Image out;
	mergeFieldGeneric(srcArr, out);
	return out;
}

ImagePair Image::splitStereo(Enum<MergeMode> srcMergeMode)
{
    SPADAS_ERROR_RETURNVAL(!vars, ImagePair());
    SPADAS_ERROR_RETURNVAL(!MergeMode::is3DMerged(srcMergeMode), ImagePair());
    SPADAS_ERROR_RETURNVAL(height() % 2 != 0 && (srcMergeMode == MergeMode::LineByLineLR || srcMergeMode == MergeMode::LineByLineRL), ImagePair());
    SPADAS_ERROR_RETURNVAL(width() % 2 != 0 && (srcMergeMode == MergeMode::FullSideBySide || srcMergeMode == MergeMode::HalfSideBySide), ImagePair());

	Image dst[2];
	switch (vars->format.value())
	{
        case PixelFormat::ByteBGR:
        case PixelFormat::ByteGray:
        case PixelFormat::ByteRGB:
        case PixelFormat::ByteYUV:
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
            splitStereoByte(*this, dst, srcMergeMode, PixelFormat::nChannels(vars->format));
            break;
        case PixelFormat::WordBGR:
        case PixelFormat::WordGray:
            splitStereoWord(*this, dst, srcMergeMode, PixelFormat::nChannels(vars->format));
            break;
        default:
			SPADAS_ERROR_MSG("Invalid vars->format");
			break;
	}
    return ImagePair(dst[0], dst[1]);
}

Image Image::mergeStereo(ImagePair src, Enum<MergeMode> dstMergeMode)
{
	if (!src.image1.vars && !src.image2.vars) return Image();

	if (!src.image1.vars) src.image1 = Image(src.image2.size(), src.image2.format());
	if (!src.image2.vars) src.image2 = Image(src.image1.size(), src.image1.format());

    SPADAS_ERROR_RETURNVAL(src.image1.size() != src.image2.size(), Image());
    SPADAS_ERROR_RETURNVAL(src.image1.format() != src.image2.format(), Image());
    
    Size2D srcSize = src.image1.size();
    Enum<PixelFormat> srcFormat = src.image1.format();
    
    SPADAS_ERROR_RETURNVAL(dstMergeMode == MergeMode::HalfSideBySide && srcSize.width % 2 != 0, Image());
    SPADAS_ERROR_RETURNVAL((dstMergeMode == MergeMode::LineByLineLR || dstMergeMode == MergeMode::LineByLineRL) && srcSize.height % 2 != 0, Image());

    Image srcArr[2] = {src.image1, src.image2};
	Image dst;
	switch (srcFormat.value())
	{
	case PixelFormat::ByteBGR:
	case PixelFormat::ByteGray:
    case PixelFormat::ByteRGB:
    case PixelFormat::ByteYUV:
    case PixelFormat::ByteBGRA:
    case PixelFormat::ByteRGBA:
        mergeStereoByte(srcArr, dst, dstMergeMode, PixelFormat::nChannels(srcFormat));
        break;
	case PixelFormat::WordBGR:
	case PixelFormat::WordGray:
		mergeStereoWord(srcArr, dst, dstMergeMode, PixelFormat::nChannels(srcFormat));
		break;
	default:
		SPADAS_ERROR_MSG("Invalid srcFormat");
        break;
	}
	return dst;
}

Array<Image> Image::splitChannels()
{
    SPADAS_ERROR_RETURNVAL(!vars, Array<Image>());

	/* check formats of outputs */
	Enum<PixelFormat> dstFormat;
	switch (vars->format.value())
	{
	case PixelFormat::ByteBGR:
	case PixelFormat::ByteRGB:
    case PixelFormat::ByteYUV:
    case PixelFormat::ByteBGRA:
    case PixelFormat::ByteRGBA:
		dstFormat = PixelFormat::ByteGray;
		break;
	case PixelFormat::WordBGR:
		dstFormat = PixelFormat::WordGray;
		break;
	case PixelFormat::FloatBGR:
	case PixelFormat::FloatHSV:
		dstFormat = PixelFormat::FloatGray;
		break;
	default:
		SPADAS_ERROR_MSG("Invalid vars->format");
		return Array<Image>();
	}

    UInt nChannels = PixelFormat::nChannels(vars->format);
    Array<Image> channels(nChannels);
	for (UInt i = 0; i < nChannels; i++) channels[i] = Image(Size2D::wh(vars->width, vars->height), dstFormat, FALSE);

	/* split */
    for (UInt v = 0; v < vars->height; v++)
    {
        PixelData srcRow = operator[](v);
        PixelData c0Row = channels[0][v];
        PixelData c1Row = channels[1][v];
        PixelData c2Row = channels[2][v];
        PixelData c3Row = nChannels == 4 ? channels[3][v] : PixelData();
        
        UInt i = 0;
        switch (vars->format.value())
        {
        case PixelFormat::ByteBGR:
        case PixelFormat::ByteRGB:
        case PixelFormat::ByteYUV:
            for (UInt u = 0; u < vars->width; u++)
            {
                c0Row.b[u] = srcRow.b[i++];
                c1Row.b[u] = srcRow.b[i++];
                c2Row.b[u] = srcRow.b[i++];
            }
            break;
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
			for (UInt u = 0; u < vars->width; u++)
			{
				c0Row.b[u] = srcRow.b[i++];
				c1Row.b[u] = srcRow.b[i++];
				c2Row.b[u] = srcRow.b[i++];
				c3Row.b[u] = srcRow.b[i++];
			}
			break;
        case PixelFormat::WordBGR:
            for (UInt u = 0; u < vars->width; u++)
            {
                c0Row.w[u] = srcRow.w[i++];
                c1Row.w[u] = srcRow.w[i++];
                c2Row.w[u] = srcRow.w[i++];
            }
            break;
        case PixelFormat::FloatBGR:
            for (UInt u = 0; u < vars->width; u++)
            {
                c0Row.f[u] = srcRow.f[i++];
                c1Row.f[u] = srcRow.f[i++];
                c2Row.f[u] = srcRow.f[i++];
            }
            break;
        case PixelFormat::FloatHSV:
            for (UInt u = 0; u < vars->width; u++)
            {
                c0Row.f[u] = clamp(srcRow.f[i++] / 360, 0.0f, 1.0f);
                c1Row.f[u] = srcRow.f[i++];
                c2Row.f[u] = srcRow.f[i++];
            }
            break;
        default:
            break;
        }
    }
    return channels;
}

Image Image::mergeChannels(Array<Image> channels, Enum<PixelFormat> multiChannelFormat)
{
	for (UInt i = 0; i < channels.size(); i++)
	{
		if (!channels[i].vars) return Image();
	}

	/* check formats of inputs */
	Enum<PixelFormat> srcFormat;
	switch (multiChannelFormat.value())
	{
        case PixelFormat::ByteBGR:
        case PixelFormat::ByteRGB:
        case PixelFormat::ByteYUV:
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
            srcFormat = PixelFormat::ByteGray;
            break;
        case PixelFormat::WordBGR:
            srcFormat = PixelFormat::WordGray;
            break;
        case PixelFormat::FloatBGR:
        case PixelFormat::FloatHSV:
            srcFormat = PixelFormat::FloatGray;
            break;
        default:
			SPADAS_ERROR_MSG("Invalid multiChannelFormat");
            return Image();
	}

    UInt nChannels = PixelFormat::nChannels(multiChannelFormat);
    SPADAS_ERROR_RETURNVAL(channels.size() != nChannels, Image());

    Size2D srcSize = channels[0].size();
	for (UInt i = 0; i < nChannels; i++)
	{
        SPADAS_ERROR_RETURNVAL(channels[i].size() != srcSize, Image());
        SPADAS_ERROR_RETURNVAL(channels[i].format() != srcFormat, Image());
	}

    Image output(srcSize, multiChannelFormat, FALSE);

	/* merge */
    for (UInt v = 0; v < srcSize.height; v++)
    {
        PixelData dstRow = output[v];
        PixelData c0Row = channels[0][v];
        PixelData c1Row = channels[1][v];
        PixelData c2Row = channels[2][v];
        PixelData c3Row = nChannels == 4 ? channels[3][v] : PixelData();
        
        UInt i = 0;
        switch (multiChannelFormat.value())
        {
        case PixelFormat::ByteBGR:
        case PixelFormat::ByteRGB:
        case PixelFormat::ByteYUV:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.b[i++] = c0Row.b[u];
                dstRow.b[i++] = c1Row.b[u];
                dstRow.b[i++] = c2Row.b[u];
            }
            break;
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.b[i++] = c0Row.b[u];
                dstRow.b[i++] = c1Row.b[u];
                dstRow.b[i++] = c2Row.b[u];
                dstRow.b[i++] = c3Row.b[u];
            }
            break;
        case PixelFormat::WordBGR:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.w[i++] = c0Row.w[u];
                dstRow.w[i++] = c1Row.w[u];
                dstRow.w[i++] = c2Row.w[u];
            }
            break;
        case PixelFormat::FloatBGR:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.f[i++] = c0Row.f[u];
                dstRow.f[i++] = c1Row.f[u];
                dstRow.f[i++] = c2Row.f[u];
            }
            break;
        case PixelFormat::FloatHSV:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.f[i++] = clamp(c0Row.f[u] * 360.0f, 0.0f, 360.0f);
                dstRow.f[i++] = c1Row.f[u];
                dstRow.f[i++] = c2Row.f[u];
            }
            break;
        default:
            break;
        }
    }
    return output;
}

Array<Image> Image::genMipmap(UInt nLevels)
{
    SPADAS_ERROR_RETURNVAL(!vars, Array<Image>());

	if (nLevels == 0) return Array<Image>();
	
	Int width = (Int)vars->width;
	Int height = (Int)vars->height;
	
	Int maxMultiplier = math::round(math::power(2.0f, (Float)nLevels-1));
    SPADAS_ERROR_RETURNVAL(width % maxMultiplier != 0 || height % maxMultiplier != 0, Array<Image>());
	
    Array<Image> mipmap(nLevels);
	mipmap[0] = clone();
	for (UInt i = 1; i < nLevels; i++)
	{
        mipmap[i] = mipmap[i-1].resize(ResizeScale::Half);
	}
    return mipmap;
}

String Image::getInfoText()
{
	if (!vars) return "(null)";
	else return (String)"Width: " + vars->width + "\nHeight: " + vars->height + "\nPixel format: " + vars->format.toString() + "\nRow offset bytes: " + vars->rowBytes + "\nBytes per pixel: " + vars->bytesPerPixel;
}
