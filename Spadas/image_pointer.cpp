
#include "spadas.h"

#include "image_bz.h"

using namespace image_internal;

ImagePointer::ImagePointer() : width(DEFAULT_SIZE), height(DEFAULT_SIZE), isColor(FALSE), \
	data(calcImageDataSize(Size2D::wh(DEFAULT_SIZE, DEFAULT_SIZE), PixelFormat::ByteGray))
{
	rowBytes = data.size() * 8 / height;
}
ImagePointer::ImagePointer(Size2D size, Bool isColor) : width(math::clamp(size.width, 1u, MAX_WIDTH)), height(math::clamp(size.height, 1u, MAX_HEIGHT)), isColor(isColor), \
	data(calcImageDataSize(Size2D::wh(math::clamp(size.width, 1u, MAX_WIDTH), math::clamp(size.height, 1u, MAX_HEIGHT)), isColor ? PixelFormat::ByteBGR : PixelFormat::ByteGray))
{
	rowBytes = data.size() * 8 / height;
}