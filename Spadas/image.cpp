#define SPADAS_ENABLE_MACROS
#include "image.h"

namespace image_internal
{
	using namespace spadas;
    
    ColorConvertTable table;
}

using namespace spadas;
using namespace spadas::math;
using namespace spadas::console;
using namespace spadas::utility;
using namespace image_internal;

Image::Image()
{
}

Image::Image(Size2D size, Enum<PixelFormat> format, Bool setPixels)
{
    SPADAS_ERROR_RETURN(size.width == 0 || size.width > MAX_WIDTH);
    SPADAS_ERROR_RETURN(size.height == 0 || size.height > MAX_HEIGHT);

	setVars(new ImageVars(), TRUE);
    var()->width = size.width;
    var()->height = size.height;
    var()->format = format;
    var()->span = Binary(calcImageByteSize(size, format)).span();
    var()->rowBytes = var()->span.size() / var()->height;
    var()->bytesPerPixel = PixelFormat::bytesPerPixel(var()->format);
    if (setPixels) var()->span.set(0);
}

Image::Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Bool setPixels)
{
    operator =(Image(ImageResolution::size(resolution), format, setPixels));
}

Image::Image(ImagePointer pointer)
{
    SPADAS_ERROR_RETURN(!pointer.isValid());

    setVars(new ImageVars(), TRUE);
	var()->width = pointer.getWidth();
	var()->height = pointer.getHeight();
	var()->format = pointer.isColor() ? PixelFormat::Value::ByteBGR : PixelFormat::Value::ByteGray;
    var()->span = pointer.getData();
	var()->rowBytes = pointer.getRowBytes();
	var()->bytesPerPixel = PixelFormat::bytesPerPixel(var()->format);
}

Image::Image(Enum<ImageFileFormat> format, Binary fileData)
{
    SPADAS_ERROR_RETURN(fileData.isEmpty());

    switch (format.value())
    {
    case ImageFileFormat::Value::BMP:
        operator =(decodeBMP(fileData));
        break;
    case ImageFileFormat::Value::JPG:
        operator =(decodeJPG(fileData));
        break;
    case ImageFileFormat::Value::PNG:
        operator =(decodePNG(fileData));
        break;
    default:
        SPADAS_ERROR_MSG("Invalid format");
        break;
    }
}

Image::Image(Path filePath)
{
    SPADAS_ERROR_RETURN(filePath.isNull() || filePath.isFolder());
    SPADAS_ERROR_RETURN(!filePath.exist());

    String ext = filePath.extension();
    SPADAS_ERROR_RETURN(ext != ".bmp" && ext != ".jpg" && ext != ".jpeg" && ext != ".png");

	File file = File::openBinary(filePath);
    SPADAS_ERROR_RETURN(file.isNull());

	Binary fileData = file.input();
    SPADAS_ERROR_RETURN(fileData.isEmpty());

    Enum<ImageFileFormat> format;
    if (ext == ".bmp") format = ImageFileFormat::Value::BMP;
    else if (ext == ".jpg" || ext == ".jpeg") format = ImageFileFormat::Value::JPG;
    else if (ext == ".png") format = ImageFileFormat::Value::PNG;
    else return;

    operator =(Image(format, fileData));
}

Bool Image::isRefCounting()
{
    SPADAS_ERROR_RETURNVAL(!vars, FALSE);
    return var()->span.isRefCounting();
}

Image Image::clone()
{
	SPADAS_ERROR_RETURNVAL(!vars, Image());
    Size2D size = Size2D::wh(var()->width, var()->height);
    CoordInt2D offset = CoordInt2D::uv(0, 0);
    Image out(size, var()->format, FALSE);
    out.copy(*this, Region2D(offset, size), offset);
    return out;
}

void Image::copy(Image src, Region2D srcRegion, CoordInt2D thisOffset)
{
	SPADAS_ERROR_RETURN(!vars);
	SPADAS_ERROR_RETURN(!src.vars || vars == src.vars);
	SPADAS_ERROR_RETURN(var()->format != src.format());
    SPADAS_ERROR_RETURN(srcRegion.width == 0 || srcRegion.width > MAX_WIDTH);
    SPADAS_ERROR_RETURN(srcRegion.height == 0 || srcRegion.height > MAX_HEIGHT);
	SPADAS_ERROR_RETURN(srcRegion.offsetU < 0 || srcRegion.offsetV < 0 || srcRegion.offsetU + srcRegion.width > src.width() || srcRegion.offsetV + srcRegion.height > src.height());
	SPADAS_ERROR_RETURN(thisOffset.u < 0 || thisOffset.v < 0 || thisOffset.u + srcRegion.width > var()->width || thisOffset.v + srcRegion.height > var()->height);

    Byte *srcData = src.data().b;
    UInt srcRowBytes = src.rowBytes();
    Byte *dstData = var()->span.data();
    UInt rowBytesValid = srcRegion.width * var()->bytesPerPixel;
    for (UInt vSrc = srcRegion.offsetV, vDst = thisOffset.v; vSrc < srcRegion.offsetV + srcRegion.height; vSrc++, vDst++)
    {
        Byte *srcRow = &srcData[vSrc * srcRowBytes + srcRegion.offsetU * var()->bytesPerPixel];
        Byte *dstRow = &dstData[vDst * var()->rowBytes + thisOffset.u * var()->bytesPerPixel];
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
        case PixelFormat::Value::ByteBGRA:
            srcFormat = PixelFormat::Value::ByteBGR;
            break;
        case PixelFormat::Value::ByteRGBA:
            srcFormat = PixelFormat::Value::ByteRGB;
            break;
        default:
			SPADAS_ERROR_MSG("Invalid layer.format()");
            return;
    }
    
	SPADAS_ERROR_RETURN(var()->format != srcFormat);
    
    Size2D layerSize = layer.size();
    if (offset.u >= (Int)var()->width || offset.v >= (Int)var()->height || offset.u + (Int)layerSize.width <= 0 || offset.v + (Int)layerSize.height <= 0) return;
    
    UInt uBegin = (UInt)math::max(0, offset.u);
    UInt vBegin = (UInt)math::max(0, offset.v);
    UInt uEnd = (UInt)math::min((Int)var()->width-1, offset.u + (Int)layerSize.width - 1);
    UInt vEnd = (UInt)math::min((Int)var()->height-1, offset.v + (Int)layerSize.height - 1);
    
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
	SPADAS_ERROR_RETURNVAL(var()->format == PixelFormat::Value::ByteUYVY && (region.offsetU % 2 == 1 || region.width % 2 == 1), Image());
    SPADAS_ERROR_RETURNVAL(region.width == 0 || region.width > MAX_WIDTH, Image());
    SPADAS_ERROR_RETURNVAL(region.height == 0 || region.height > MAX_HEIGHT, Image());
	SPADAS_ERROR_RETURNVAL(region.offsetU < 0 || region.offsetV < 0 || region.offsetU + region.width > var()->width || region.offsetV + region.height > var()->height, Image());
	
    BinarySpan subSpan = var()->span.span(region.offsetV * var()->rowBytes + region.offsetU * var()->bytesPerPixel, region.height * var()->rowBytes);
    SPADAS_ERROR_RETURNVAL(subSpan.size() < (region.height - 1) * var()->rowBytes + region.width * var()->bytesPerPixel, Image());

    ImageVars *newVars = new ImageVars();
    newVars->width = region.width;
    newVars->height = region.height;
    newVars->format = var()->format;
    newVars->rowBytes = var()->rowBytes;
    newVars->bytesPerPixel = var()->bytesPerPixel;
    newVars->span = subSpan;
	
	Image out;
	out.setVars(newVars, TRUE);
    return out;
}

ImagePointer Image::imagePointer()
{
	SPADAS_ERROR_RETURNVAL(!vars, ImagePointer());
    SPADAS_ERROR_RETURNVAL(var()->format != PixelFormat::Value::ByteBGR && var()->format != PixelFormat::Value::ByteGray, ImagePointer());

    return ImagePointer(Size2D::wh(var()->width, var()->height), var()->format == PixelFormat::Value::ByteBGR, var()->rowBytes, var()->span);
}

Size2D Image::size()
{
	SPADAS_ERROR_RETURNVAL(!vars, Size2D());
	return Size2D::wh(var()->width, var()->height);
}

UInt Image::width()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return var()->width;
}

UInt Image::height()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return var()->height;
}

Enum<PixelFormat> Image::format()
{
    SPADAS_ERROR_RETURNVAL(!vars, Enum<PixelFormat>());
	return var()->format;
}

UInt Image::rowBytes()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return var()->rowBytes;
}

UInt Image::rowBytesValid()
{
    SPADAS_ERROR_RETURNVAL(!vars, 0);
	return var()->width * var()->bytesPerPixel;
}

PixelData Image::data()
{
    SPADAS_ERROR_RETURNVAL(!vars, PixelData());
	return PixelData(var()->span.data(), var()->bytesPerPixel);
}

PixelData Image::operator[](UInt v)
{
    SPADAS_ERROR_RETURNVAL(!vars, PixelData());
    SPADAS_ERROR_RETURNVAL(v >= var()->height, PixelData());
	return PixelData((Pointer)(var()->span.data() + v * var()->rowBytes), var()->bytesPerPixel);
}

void Image::forPixels(Func<void(UInt, UInt, PixelData)> func)
{
    SPADAS_ERROR_RETURN(!vars);
    SPADAS_ERROR_RETURN(!func);

    for (UInt v = 0; v < var()->height; v++)
    {
        Byte *rowPtr = var()->span.data() + v * var()->rowBytes;
        for (UInt u = 0; u < var()->width; u++, rowPtr += var()->bytesPerPixel)
        {
            func(v, u, PixelData(rowPtr, var()->bytesPerPixel));
        }
    }
}

Image Image::resize(Enum<ResizeScale> resizeScale)
{
    SPADAS_ERROR_RETURNVAL(!vars, Image());

	if (var()->format != PixelFormat::Value::ByteBGR &&
        var()->format != PixelFormat::Value::ByteGray &&
        var()->format != PixelFormat::Value::WordBGR &&
        var()->format != PixelFormat::Value::WordGray &&
        var()->format != PixelFormat::Value::ByteRGB &&
        var()->format != PixelFormat::Value::ByteYUV &&
        var()->format != PixelFormat::Value::ByteBGRA &&
        var()->format != PixelFormat::Value::ByteRGBA)
	{
		SPADAS_ERROR_MSG("Invalid var()->format");
		return Image();
	}
	
	Int scale = 1;
	Float scaleF = 1.0f;
	switch (resizeScale.value())
	{
		case ResizeScale::Value::Quarter:
			scale = 4;
			scaleF = 4.0f;
			break;
		case ResizeScale::Value::Half:
			scale = 2;
			scaleF = 2.0f;
			break;
		case ResizeScale::Value::Double:
			scale = 1;
			scaleF = 0.5f;
			break;
        default:
            SPADAS_ERROR_MSG("Invalid resizeScale");
            return Image();
	}
	
    SPADAS_ERROR_RETURNVAL(var()->width % scale != 0 || var()->height % scale != 0, Image());
	
	Size2D targetSize = Size2D::wh(math::round((Float)var()->width / scaleF), math::round((Float)var()->height / scaleF));
	
    Image image(targetSize, var()->format, FALSE);
	switch (var()->format.value())
	{
		case PixelFormat::Value::ByteBGR:
		case PixelFormat::Value::ByteRGB:
		case PixelFormat::Value::ByteYUV:
        case PixelFormat::Value::ByteBGRA:
        case PixelFormat::Value::ByteRGBA:
            resizeColorByteWithScale(*this, image, resizeScale, PixelFormat::nChannels(var()->format));
			break;
        case PixelFormat::Value::ByteGray:
			resizeGrayByteWithScale(*this, image, resizeScale);
			break;
		case PixelFormat::Value::WordBGR:
			resizeColorWordWithScale(*this, image, resizeScale);
			break;
		case PixelFormat::Value::WordGray:
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
	if (mode == ResizeMode::Value::Nearest)
	{
		rowToRead = 1;
	}
	if (mode == ResizeMode::Value::Bilinear)
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
	
	if (mode == ResizeMode::Value::Nearest)
	{
		UInt prev = 99999;
		for (UInt y = 0; y < dst.height(); y++)
		{
			v = y * scaleHeight;
			v = (Float)math::round(v);
			if (v >= (Float)src.height() - 1)
			{
				v = (Float)src.height() - 1;
			}
			if (prev != v)
			{
				rowLoader->io(src[(Int)v].ptr, dataIn[0], src.width());
			}
			prev = (UInt)math::round(v);
			rowRsz->rsz(dataIn, dataOut, src.width(), dst.width(), src.height(), dst.height(), mode, v);
			rowWriter->io(dst[y].ptr, dataOut, dst.width());
		}
	}
	if(mode == ResizeMode::Value::Bilinear)
	{
		UInt prefloorv = 99999;
		for (UInt y = 0; y < dst.height(); y ++)
		{
			v = y * scaleHeight;
			UInt floorv = math::floor(v);
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

	if (mode == ResizeMode::Value::Nearest || mode == ResizeMode::Value::Bilinear)
	{
        table.init();
		
        Image image(outputSize, var()->format, FALSE);
		Image sub(*this);
		if (undistort)
		{
			UInt srcWidth = var()->width;
			UInt srcHeight = var()->height;
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
        if (var()->format != PixelFormat::Value::ByteBGR &&
            var()->format != PixelFormat::Value::ByteGray &&
            var()->format != PixelFormat::Value::WordBGR &&
            var()->format != PixelFormat::Value::WordGray &&
            var()->format != PixelFormat::Value::FloatBGR &&
            var()->format != PixelFormat::Value::FloatGray &&
            var()->format != PixelFormat::Value::ByteRGB &&
            var()->format != PixelFormat::Value::ByteYUV &&
            var()->format != PixelFormat::Value::ByteBGRA &&
            var()->format != PixelFormat::Value::ByteRGBA)
        {
			SPADAS_ERROR_MSG("Invalid var()->format");
            return Image();
        }
            
		Float widthRatio = (Float)var()->width / outputSize.width;
		Float heightRatio = (Float)var()->height / outputSize.height;
		
		if (undistort)
		{
			widthRatio = math::min(widthRatio, heightRatio);
			heightRatio = widthRatio;
		}
		
        Image image(outputSize, var()->format, FALSE);
		switch (var()->format.value())
		{
			case PixelFormat::Value::ByteBGR:
            case PixelFormat::Value::ByteRGB:
			case PixelFormat::Value::ByteYUV:
            case PixelFormat::Value::ByteBGRA:
            case PixelFormat::Value::ByteRGBA:
                areaResizeColorByte(*this, image, widthRatio, heightRatio, PixelFormat::nChannels(var()->format));
				break;
			case PixelFormat::Value::ByteGray:
				areaResizeGrayByte(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::Value::WordBGR:
				areaResizeColorWord(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::Value::WordGray:
				areaResizeGrayWord(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::Value::FloatBGR:
				areaResizeColorFloat(*this, image, widthRatio, heightRatio);
				break;
			case PixelFormat::Value::FloatGray:
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

	if (outputFormat == var()->format) return clone();

	table.init();
	
    Image image(Size2D::wh(var()->width, var()->height), outputFormat, FALSE);

    Bool fastConverted = TRUE;
    switch (var()->format.value())
    {
        case PixelFormat::Value::ByteBGR:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteGray:
                fastCvtByteBGR2ByteGray(*this, image);
                break;
            case PixelFormat::Value::FloatBGR:
                fastCvtByteBGR2FloatBGR(*this, image);
                break;
            case PixelFormat::Value::FloatGray:
                fastCvtByteBGR2FloatGray(*this, image);
                break;
            case PixelFormat::Value::WordBGR:
                fastCvtByteBGR2WordBGR(*this, image);
                break;
            case PixelFormat::Value::WordGray:
                fastCvtByteBGR2WordGray(*this, image);
                break;
            case PixelFormat::Value::ByteRGB:
                fastCvtByteBGR2ByteRGB(*this, image);
                break;
            case PixelFormat::Value::ByteUYVY:
                fastCvtByteBGR2ByteUYVY(*this, image);
                break;
            case PixelFormat::Value::ByteYUV:
                fastCvtByteBGR2ByteYUV(*this, image);
                break;
            case PixelFormat::Value::ByteBGRA:
                fastCvtByteBGR2ByteBGRA(*this, image);
                break;
            case PixelFormat::Value::ByteRGBA:
                fastCvtByteBGR2ByteRGBA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::ByteGray:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtByteGray2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::FloatGray:
                fastCvtByteGray2FloatGray(*this, image);
                break;
            case PixelFormat::Value::WordGray:
                fastCvtByteGray2WordGray(*this, image);
                break;
            case PixelFormat::Value::ByteUYVY:
                fastCvtByteGray2ByteUYVY(*this, image);
                break;
            case PixelFormat::Value::ByteYUV:
                fastCvtByteGray2ByteYUV(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::FloatBGR:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtFloatBGR2ByteBGR(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::FloatGray:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtFloatGray2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteGray:
                fastCvtFloatGray2ByteGray(*this, image);
                break;
            case PixelFormat::Value::WordGray:
                fastCvtFloatGray2WordGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::WordBGR:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtWordBGR2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::WordGray:
                fastCvtWordBGR2WordGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::WordGray:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtWordGray2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteGray:
                fastCvtWordGray2ByteGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::ByteRGB:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtByteRGB2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteRGBA:
                fastCvtByteRGB2ByteRGBA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::ByteUYVY:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtByteUYVY2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteGray:
                fastCvtByteUYVY2ByteGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::ByteYUV:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtByteYUV2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteGray:
                fastCvtByteYUV2ByteGray(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::ByteBGRA:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtByteBGRA2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteRGBA:
                fastCvtByteBGRA2ByteRGBA(*this, image);
                break;
            default:
                fastConverted = FALSE;
                break;
        }
            break;
        case PixelFormat::Value::ByteRGBA:
            switch (outputFormat.value())
        {
            case PixelFormat::Value::ByteBGR:
                fastCvtByteRGBA2ByteBGR(*this, image);
                break;
            case PixelFormat::Value::ByteRGB:
                fastCvtByteRGBA2ByteRGB(*this, image);
                break;
            case PixelFormat::Value::ByteBGRA:
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
        RowIOType srcType = getRowIOType(var()->format);
        RowIOType dstType = getRowIOType(outputFormat);
        
        IRowIO *rowLoader = createRowLoader(var()->format);
        IRowIO *rowWriter = createRowWriter(outputFormat);
        IRowCvt *rowCvt = dstType == srcType ? NULL : createRowCvt(srcType, dstType);
        
        Pointer dataIn = createRowData(srcType, var()->width);
        Pointer dataOut = dstType == srcType ? dataIn : createRowData(dstType, var()->width);
        
        for (UInt v = 0; v < var()->height; v++)
        {
            rowLoader->io(operator[](v).ptr, dataIn, var()->width);
            if (dstType != srcType) rowCvt->cvt(dataIn, dataOut, var()->width);
            rowWriter->io(image[v].ptr, dataOut, var()->width);
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

    if (leftRightFlip && upDownFlip && var()->format != PixelFormat::Value::ByteUYVY) return rotate(ImageRotation::Value::CW180);
	
	table.init();
	
    Image image(Size2D::wh(var()->width, var()->height), var()->format, FALSE);
	
	if (leftRightFlip)
	{
        RowIOType type = getRowIOType(var()->format);
        
        IRowIO *rowLoader = createRowLoader(var()->format);
        IRowIO *rowWriter = createRowWriter(var()->format);
        IRowCvt *rowFlip = createRowFlip(type);
        
        Pointer dataIn = createRowData(type, var()->width);
        Pointer dataOut = createRowData(type, var()->width);
        
        if (upDownFlip)
        {
            for (UInt vSrc = 0, vDst = var()->height - 1; vSrc < var()->height; vSrc++, vDst--)
            {
                rowLoader->io(operator[](vSrc).ptr, dataIn, var()->width);
                rowFlip->cvt(dataIn, dataOut, var()->width);
                rowWriter->io(image[vDst].ptr, dataOut, var()->width);
            }
        }
        else
        {
            for (UInt v = 0; v < var()->height; v++)
            {
                rowLoader->io(operator[](v).ptr, dataIn, var()->width);
                rowFlip->cvt(dataIn, dataOut, var()->width);
                rowWriter->io(image[v].ptr, dataOut, var()->width);
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
			for (UInt vSrc = 0, vDst = var()->height - 1; vSrc < var()->height; vSrc++, vDst--)
			{
				memoryCopy(operator[](vSrc).ptr, image[vDst].ptr, rowBytes);
			}
		}
		else
		{
			for (UInt v = 0; v < var()->height; v++)
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
    SPADAS_ERROR_RETURNVAL(var()->format == PixelFormat::Value::ByteUYVY, Image());
    SPADAS_ERROR_RETURNVAL(rotation == ImageRotation::Value::None, Image());

	Size2D targetSize;
	if (rotation == ImageRotation::Value::CW180) targetSize = this->size();
	else targetSize = Size2D::wh(var()->height, var()->width);
    
    Image image(targetSize, var()->format, FALSE);
	
	switch (var()->format.value())
	{
		case PixelFormat::Value::ByteBGR:
		case PixelFormat::Value::ByteGray:
        case PixelFormat::Value::ByteRGB:
		case PixelFormat::Value::ByteYUV:
        case PixelFormat::Value::ByteBool:
        case PixelFormat::Value::ByteBGRA:
        case PixelFormat::Value::ByteRGBA:
			rotateByte(*this, image, PixelFormat::nChannels(var()->format), rotation);
			break;
		case PixelFormat::Value::WordBGR:
		case PixelFormat::Value::WordGray:
			rotateWord(*this, image, PixelFormat::isColor(var()->format), rotation);
			break;
		case PixelFormat::Value::FloatBGR:
		case PixelFormat::Value::FloatGray:
		case PixelFormat::Value::FloatHSV:
			rotateInt(*this, image, PixelFormat::isColor(var()->format), rotation);
			break;
		default:
			break;
	}
    return image;
}

Array<Image> Image::splitChannels()
{
    SPADAS_ERROR_RETURNVAL(!vars, Array<Image>());

	/* check formats of outputs */
	Enum<PixelFormat> dstFormat;
	switch (var()->format.value())
	{
	case PixelFormat::Value::ByteBGR:
	case PixelFormat::Value::ByteRGB:
    case PixelFormat::Value::ByteYUV:
    case PixelFormat::Value::ByteBGRA:
    case PixelFormat::Value::ByteRGBA:
		dstFormat = PixelFormat::Value::ByteGray;
		break;
	case PixelFormat::Value::WordBGR:
		dstFormat = PixelFormat::Value::WordGray;
		break;
	case PixelFormat::Value::FloatBGR:
	case PixelFormat::Value::FloatHSV:
		dstFormat = PixelFormat::Value::FloatGray;
		break;
	default:
		SPADAS_ERROR_MSG("Invalid var()->format");
		return Array<Image>();
	}

    UInt nChannels = PixelFormat::nChannels(var()->format);
    Array<Image> channels(nChannels);
	for (UInt i = 0; i < nChannels; i++) channels[i] = Image(Size2D::wh(var()->width, var()->height), dstFormat, FALSE);

	/* split */
    for (UInt v = 0; v < var()->height; v++)
    {
        PixelData srcRow = operator[](v);
        PixelData c0Row = channels[0][v];
        PixelData c1Row = channels[1][v];
        PixelData c2Row = channels[2][v];
        PixelData c3Row = nChannels == 4 ? channels[3][v] : PixelData();
        
        UInt i = 0;
        switch (var()->format.value())
        {
        case PixelFormat::Value::ByteBGR:
        case PixelFormat::Value::ByteRGB:
        case PixelFormat::Value::ByteYUV:
            for (UInt u = 0; u < var()->width; u++)
            {
                c0Row.b[u] = srcRow.b[i++];
                c1Row.b[u] = srcRow.b[i++];
                c2Row.b[u] = srcRow.b[i++];
            }
            break;
        case PixelFormat::Value::ByteBGRA:
        case PixelFormat::Value::ByteRGBA:
			for (UInt u = 0; u < var()->width; u++)
			{
				c0Row.b[u] = srcRow.b[i++];
				c1Row.b[u] = srcRow.b[i++];
				c2Row.b[u] = srcRow.b[i++];
				c3Row.b[u] = srcRow.b[i++];
			}
			break;
        case PixelFormat::Value::WordBGR:
            for (UInt u = 0; u < var()->width; u++)
            {
                c0Row.w[u] = srcRow.w[i++];
                c1Row.w[u] = srcRow.w[i++];
                c2Row.w[u] = srcRow.w[i++];
            }
            break;
        case PixelFormat::Value::FloatBGR:
            for (UInt u = 0; u < var()->width; u++)
            {
                c0Row.f[u] = srcRow.f[i++];
                c1Row.f[u] = srcRow.f[i++];
                c2Row.f[u] = srcRow.f[i++];
            }
            break;
        case PixelFormat::Value::FloatHSV:
            for (UInt u = 0; u < var()->width; u++)
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
        case PixelFormat::Value::ByteBGR:
        case PixelFormat::Value::ByteRGB:
        case PixelFormat::Value::ByteYUV:
        case PixelFormat::Value::ByteBGRA:
        case PixelFormat::Value::ByteRGBA:
            srcFormat = PixelFormat::Value::ByteGray;
            break;
        case PixelFormat::Value::WordBGR:
            srcFormat = PixelFormat::Value::WordGray;
            break;
        case PixelFormat::Value::FloatBGR:
        case PixelFormat::Value::FloatHSV:
            srcFormat = PixelFormat::Value::FloatGray;
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
        case PixelFormat::Value::ByteBGR:
        case PixelFormat::Value::ByteRGB:
        case PixelFormat::Value::ByteYUV:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.b[i++] = c0Row.b[u];
                dstRow.b[i++] = c1Row.b[u];
                dstRow.b[i++] = c2Row.b[u];
            }
            break;
        case PixelFormat::Value::ByteBGRA:
        case PixelFormat::Value::ByteRGBA:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.b[i++] = c0Row.b[u];
                dstRow.b[i++] = c1Row.b[u];
                dstRow.b[i++] = c2Row.b[u];
                dstRow.b[i++] = c3Row.b[u];
            }
            break;
        case PixelFormat::Value::WordBGR:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.w[i++] = c0Row.w[u];
                dstRow.w[i++] = c1Row.w[u];
                dstRow.w[i++] = c2Row.w[u];
            }
            break;
        case PixelFormat::Value::FloatBGR:
            for (UInt u = 0; u < srcSize.width; u++)
            {
                dstRow.f[i++] = c0Row.f[u];
                dstRow.f[i++] = c1Row.f[u];
                dstRow.f[i++] = c2Row.f[u];
            }
            break;
        case PixelFormat::Value::FloatHSV:
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
	
	Int width = (Int)var()->width;
	Int height = (Int)var()->height;
	
	Int maxMultiplier = (Int)math::round(math::power(2.0f, (Float)nLevels - 1));
    SPADAS_ERROR_RETURNVAL(width % maxMultiplier != 0 || height % maxMultiplier != 0, Array<Image>());
	
    Array<Image> mipmap(nLevels);
	mipmap[0] = clone();
	for (UInt i = 1; i < nLevels; i++)
	{
        mipmap[i] = mipmap[i-1].resize(ResizeScale::Value::Half);
	}
    return mipmap;
}

Binary Image::toBinary(Enum<ImageFileFormat> format, UInt jpgQuality)
{
    SPADAS_ERROR_RETURNVAL(!vars, Binary());

    Binary output;
    switch (format.value())
    {
    case ImageFileFormat::Value::BMP:
        output = encodeBMP(*this);
        break;
    case ImageFileFormat::Value::JPG:
        SPADAS_ERROR_RETURNVAL(jpgQuality > 100, Binary());
        output = encodeJPG(*this, jpgQuality);
        break;
    case ImageFileFormat::Value::PNG:
        output = encodePNG(*this);
        break;
    default:
        SPADAS_ERROR_MSG("Invalid format");
        break;
    }

    return output;
}

void Image::save(Path filePath, UInt jpgQuality)
{
    SPADAS_ERROR_RETURN(!vars);
    SPADAS_ERROR_RETURN(filePath.isNull() || filePath.isFolder());

    String ext = filePath.extension();
    SPADAS_ERROR_RETURN(ext != ".bmp" && ext != ".jpg" && ext != ".jpeg" && ext != ".png");
    
    Enum<ImageFileFormat> format;
    if (ext == ".bmp") format = ImageFileFormat::Value::BMP;
    else if (ext == ".jpg" || ext == ".jpeg") format = ImageFileFormat::Value::JPG;
    else if (ext == ".png") format = ImageFileFormat::Value::PNG;
    else return;

    Binary fileData = toBinary(format, jpgQuality);
    if (fileData.isEmpty()) return;

	File file = File::createBinary(filePath);
	SPADAS_ERROR_RETURN(file.isNull());
    
	file.output(fileData);
}

String Image::getInfoText()
{
	if (!vars) return "(null)";
	else return (String)"Width: " + var()->width + "\nHeight: " + var()->height + "\nPixel format: " + var()->format.toString() + "\nRow offset bytes: " + var()->rowBytes + "\nBytes per pixel: " + var()->bytesPerPixel;
}
