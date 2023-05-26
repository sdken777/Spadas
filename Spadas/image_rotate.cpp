
#include "image.h"

namespace image_internal
{
	template<typename Type>
	void rotateSingleUnit(Image src, Image& dst, Enum<ImageRotation> rotation)
	{
		UInt width = src.width(), height = src.height();
		UInt rowUnits = dst.rowBytes() / sizeof(Type);
		switch (rotation.value())
		{
			case ImageRotation::Value::CW90:
				{
					Type *dstData = (Type*)dst.data().ptr;
					for (UInt v = 0; v < height; v++)
					{
						Type *srcRow = (Type*)src[v].ptr;
						Type *dstCol = &dstData[height-1-v];
						for (UInt uSrc = 0, uDst = 0; uSrc < width; uSrc++, uDst += rowUnits)
						{
							dstCol[uDst] = srcRow[uSrc];
						}
					}
				}
				break;
				
			case ImageRotation::Value::CCW90:
				{
					Type *dstData = (Type*)dst.data().ptr;
					UInt rowUnits = dst.rowBytes() / sizeof(Type);
					for (UInt v = 0; v < height; v++)
					{
						Type *srcRow = (Type*)src[v].ptr;
						Type *dstCol = &dstData[v];
						for (Int uSrc = (Int)width-1, uDst = 0; uSrc >= 0; uSrc--, uDst += rowUnits)
						{
							dstCol[uDst] = srcRow[uSrc];
						}
					}
				}
				break;
				
			case ImageRotation::Value::CW180:
				{
					for (UInt v = 0; v < height; v++)
					{
						Type *srcRow = (Type*)src[v].ptr;
						Type *dstRow = (Type*)dst[height-1-v].ptr;
						for (UInt uDst = 0, uSrc = width-1; uDst < width; uDst++, uSrc--)
						{
							dstRow[uDst] = srcRow[uSrc];
						}
					}
				}
				break;

			default:
				break;
		}
	}
	
	template<typename Type>
	void rotateMultipleUnits(Image src, Image& dst, Enum<ImageRotation> rotation, UInt nChannels)
	{
		UInt width = src.width(), height = src.height();
		UInt rowUnits = dst.rowBytes() / sizeof(Type);
		switch (rotation.value())
		{
			case ImageRotation::Value::CW90:
				{
					Type *dstData = (Type*)dst.data().ptr;
					for (UInt v = 0; v < height; v++)
					{
						Type *srcRow = (Type*)src[v].ptr;
						Type *dstCell = &dstData[(height-1-v)*nChannels];
						for (UInt u = 0, un = 0; u < width; u++)
						{
							for (UInt n = 0; n < nChannels; n++) dstCell[n] = srcRow[un++];
							dstCell = &dstCell[rowUnits];
						}
					}
				}
				break;
				
			case ImageRotation::Value::CCW90:
				{
					Type *dstData = (Type*)dst.data().ptr;
					UInt rowUnits = dst.rowBytes() / sizeof(Type);
					for (UInt v = 0; v < height; v++)
					{
						Type *srcRow = (Type*)src[v].ptr;
						Type *dstCell = &dstData[v*nChannels];
						for (Int u = 0, un = nChannels*width-1; u < (Int)width; u++)
						{
							for (Int n = (Int)nChannels-1; n >= 0; n--) dstCell[n] = srcRow[un--];
							dstCell = &dstCell[rowUnits];
						}
					}
				}
				break;
				
			case ImageRotation::Value::CW180:
				{
					UInt uncStep = 2 * nChannels;
					for (UInt v = 0; v < height; v++)
					{
						Type *srcRow = (Type*)src[v].ptr;
						Type *dstRow = (Type*)dst[height-1-v].ptr;
						for (Int u = 0, un = 0, unc = nChannels*(width-1); u < (Int)width; u++, unc -= uncStep)
						{
							for (UInt n = 0; n < nChannels; n++) dstRow[unc++] = srcRow[un++];
						}
					}
				}
				break;

			default:
				break;
		}
	}
}

using namespace image_internal;
using namespace spadas::math;

void image_internal::rotateByte(Image src, Image& dst, UInt nChannels, Enum<ImageRotation> rotation)
{
    rotateMultipleUnits<Byte>(src, dst, rotation, nChannels);
}
void image_internal::rotateWord(Image src, Image& dst, Bool isColor, Enum<ImageRotation> rotation)
{
	if (isColor) rotateMultipleUnits<Word>(src, dst, rotation, 3);
	else rotateSingleUnit<Word>(src, dst, rotation);
}
void image_internal::rotateInt(Image src, Image& dst, Bool isColor, Enum<ImageRotation> rotation)
{
	if (isColor) rotateMultipleUnits<Int>(src, dst, rotation, 3);
	else rotateSingleUnit<Int>(src, dst, rotation);
}
