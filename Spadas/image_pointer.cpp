
#include "image.h"

using namespace image_internal;

ImagePointer::ImagePointer() : width(DEFAULT_SIZE), height(DEFAULT_SIZE), color(FALSE), data(calcImageDataSize(Size2D::wh(DEFAULT_SIZE, DEFAULT_SIZE), PixelFormat::Value::ByteGray))
{
	rowBytes = data.size() * 8 / height;
}
ImagePointer::ImagePointer(Size2D size, Bool isColor) : width(math::clamp(size.width, 1u, MAX_WIDTH)), height(math::clamp(size.height, 1u, MAX_HEIGHT)), color(isColor), \
	data(calcImageDataSize(Size2D::wh(math::clamp(size.width, 1u, MAX_WIDTH), math::clamp(size.height, 1u, MAX_HEIGHT)), isColor ? PixelFormat::Value::ByteBGR : PixelFormat::Value::ByteGray))
{
	rowBytes = data.size() * 8 / height;
}

ImagePointer::ImagePointer(Size2D size, Bool isColor, UInt rowBytes, Array<ULong> data) : width(size.width), height(size.height), rowBytes(rowBytes), color(isColor), data(data)
{
	SPADAS_ERROR_RETURN(width == 0 || width > MAX_WIDTH);
	SPADAS_ERROR_RETURN(height == 0 || height > MAX_HEIGHT);
	SPADAS_ERROR_RETURN(rowBytes < width * (isColor ? 3 : 1));
	SPADAS_ERROR_RETURN(data.size() * sizeof(ULong) < height * rowBytes);
}

UInt ImagePointer::getWidth()
{
	return width;
}

UInt ImagePointer::getHeight()
{
	return height;
}

Bool ImagePointer::isColor()
{
	return color;
}

UInt ImagePointer::getRowBytes()
{
	return rowBytes;
}

Array<ULong> ImagePointer::getData()
{
	return data;
}