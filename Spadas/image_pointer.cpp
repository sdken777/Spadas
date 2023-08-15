
#include "image.h"

using namespace image_internal;

ImagePointer::ImagePointer() : width(0), height(0), step(0), color(FALSE)
{
}
ImagePointer::ImagePointer(Size2D size, Bool isColor, Bool setPixels) : width(0), height(0), step(0), color(FALSE)
{
	SPADAS_ERROR_RETURN(size.width == 0 || size.width > MAX_WIDTH);
	SPADAS_ERROR_RETURN(size.height == 0 || size.height > MAX_HEIGHT);

	width = size.width;
	height = size.height;
	color = isColor;
	span = Binary(calcImageByteSize(size, isColor ? PixelFormat::Value::ByteBGR : PixelFormat::Value::ByteGray)).span();
	step = span.size() / height;
	if (setPixels) span.set(0);
}

ImagePointer::ImagePointer(Size2D size, Bool isColor, UInt rowBytes, BinarySpan data) : width(0), height(0), step(0), color(FALSE)
{
	SPADAS_ERROR_RETURN(size.width == 0 || size.width > MAX_WIDTH);
	SPADAS_ERROR_RETURN(size.height == 0 || size.height > MAX_HEIGHT);
	SPADAS_ERROR_RETURN(rowBytes < size.width * (isColor ? 3 : 1));
	SPADAS_ERROR_RETURN(data.size() < (size.height - 1) * rowBytes + size.width * (isColor ? 3 : 1));

	width = size.width;
	height = size.height;
	color = isColor;
	step = rowBytes;
	span = data;
}

Bool ImagePointer::isValid()
{
	return !span.isEmpty();
}

Bool ImagePointer::isRefCounting()
{
	SPADAS_ERROR_RETURNVAL(span.isEmpty(), FALSE);
	return span.isRefCounting();
}

UInt ImagePointer::getWidth()
{
	SPADAS_ERROR_PASS(width == 0);
	return width;
}

UInt ImagePointer::getHeight()
{
	SPADAS_ERROR_PASS(height == 0);
	return height;
}

Bool ImagePointer::isColor()
{
	SPADAS_ERROR_PASS(span.isEmpty());
	return color;
}

UInt ImagePointer::getRowBytes()
{
	SPADAS_ERROR_PASS(step == 0);
	return step;
}

BinarySpan ImagePointer::getData()
{
	SPADAS_ERROR_PASS(span.isEmpty());
	return span;
}