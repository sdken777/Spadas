
#include "spadas.h"

#include "image_bz.h"

using namespace image_internal;
using namespace spadas::math;
using namespace spadas::utility;

namespace image_internal
{
	
	template<typename Type>
	void mergeFSBS(Image src[2], Image& dst, UInt nChannels)
	{
        Size2D srcSize = src[0].size();
        dst = Image(Size2D::wh(srcSize.width * 2, srcSize.height), src[0].format(), FALSE);
        
		UInt dataBytes = src[0].width() * sizeof(Type) * nChannels;
		for (UInt v = 0; v < dst.height(); v++)
		{
			Pointer leftRow = src[0][v].ptr;
			Pointer rightRow = src[1][v].ptr;
			Byte *dstRow = dst[v].b;
			
			memoryCopy(leftRow, dstRow, dataBytes);
			memoryCopy(rightRow, &dstRow[dataBytes], dataBytes);
		}
	}
	
	template<typename Type>
	void mergeHSBS(Image src[2], Image& dst, UInt nChannels)
	{
        dst = Image(src[0].size(), src[0].format(), FALSE);
        
		for (UInt v = 0; v < dst.height(); v++)
		{
            PixelData leftRow = src[0][v];
            PixelData rightRow = src[1][v];
            PixelData dstRow = dst[v];
            
            for (UInt u = 0, u0 = 0; u < dst.width() / 2; u ++, u0 += 2)
            {
                Type *cellLeft1 = (Type*)leftRow[u0].ptr;
                Type *cellLeft2 = (Type*)leftRow[u0+1].ptr;
                Type *cellDst = (Type*)dstRow[u].ptr;
                
                for (UInt n = 0; n < nChannels; n++)
                {
                    cellDst[n] = (Type)(((Int)cellLeft1[n] + (Int)cellLeft2[n]) >> 1);
                }
            }
            
            for (UInt u = dst.width() / 2, u0 = 0; u < dst.width(); u++, u0 += 2)
            {
                Type *cellRight1 = (Type*)rightRow[u0].ptr;
                Type *cellRight2 = (Type*)rightRow[u0+1].ptr;
                Type *cellDst = (Type*)dstRow[u].ptr;
                
                for (UInt n = 0; n < nChannels; n++)
                {
                    cellDst[n] = (Type)(((Int)cellRight1[n] + (Int)cellRight2[n]) >> 1);
                }
            }
		}
	}
	
	void mergeLBL(Image src[2], Image& dst, Bool isLR)
	{
        dst = Image(src[0].size(), src[0].format(), FALSE);

		UInt height = dst.height();
		UInt dstRowBytesValid = dst.rowBytesValid();
		
		for (UInt v = 0; v < height; v++)
		{
            if (isLR)
            {
                if (v % 2 == 0) memoryCopy(src[0][v].ptr, dst[v].ptr, dstRowBytesValid);
                else memoryCopy(src[1][v].ptr, dst[v].ptr, dstRowBytesValid);
            }
            else
            {
                if (v % 2 == 0) memoryCopy(src[1][v].ptr, dst[v].ptr, dstRowBytesValid);
                else memoryCopy(src[0][v].ptr, dst[v].ptr, dstRowBytesValid);
            }
		}
	}

	template<typename Type>
	void splitFSBS(Image src, Image dst[2])
	{
		Size2D dstSize = Size2D::wh(src.width() / 2, src.height());
		Enum<PixelFormat> dstFormat = src.format();
		
		for (UInt i = 0; i < 2; i++)
		{
            dst[i] = Image(dstSize, dstFormat, FALSE);
		}
		
        dst[0].copy(src, Region2D(CoordInt2D::uv(0, 0), Size2D::wh(dstSize.width, dstSize.height)), CoordInt2D::uv(0, 0));
        dst[1].copy(src, Region2D(CoordInt2D::uv(dstSize.width, 0), Size2D::wh(dstSize.width, dstSize.height)), CoordInt2D::uv(0, 0));
	}

	template<typename Type>
	void splitHSBS(Image src, Image dst[2], UInt nChannels)
	{
		for (UInt i = 0; i < 2; i++)
		{
            dst[i] = Image(src.size(), src.format(), FALSE);
		}
		
		UInt width = src.width();
		UInt width2 = width / 2;
		UInt widthN = width2 * nChannels;
		UInt height = src.height();
		
		for (UInt v = 0; v < height; v++)
		{
			Type *srcRow = (Type*)src[v].ptr;
			Type *leftRow = (Type*)dst[0][v].ptr;
			Type *rightRow = (Type*)dst[1][v].ptr;
			
            for (UInt n = 0; n < nChannels; n++)
            {
                leftRow[n] = srcRow[n];
                leftRow[nChannels * (width-1) + n] = srcRow[nChannels * (width2-1) + n];
                rightRow[n] = srcRow[nChannels * width2 + n];
                rightRow[nChannels * (width-1) + n] = srcRow[nChannels * (width-1) + n];
            }
				
            for (UInt u = 1; u < width - 1; u++)
            {
                for (UInt n = 0; n < nChannels; n++)
                {
                    leftRow[nChannels * u + n] = (srcRow[(u - 1) / 2 * nChannels + n] >> 2) + (srcRow[u / 2 * nChannels + n] >> 1) + (srcRow[(u + 1) / 2 * nChannels + n] >> 2);
                    rightRow[nChannels * u + n] = (srcRow[(u - 1) / 2 * nChannels + widthN + n] >> 2) + (srcRow[u / 2 * nChannels + widthN + n] >> 1) + (srcRow[(u + 1) / 2 * nChannels + widthN + n] >> 2);
                }
            }
		}
	}

	template<typename Type>
	void splitLBL(Image src, Image dst[2], UInt nChannels, Bool isLR, Bool floating)
	{
		for (UInt i = 0; i < 2; i++)
		{
            dst[i] = Image(src.size(), src.format(), FALSE);
		}
		
		UInt width = src.width();
		UInt height = src.height();
		UInt rowBytes = src.rowBytesValid();
		
		for (Int v = 0; v < (Int)height; v++)
		{
			Type *leftRow = (Type*)dst[0][v].ptr;
			Type *rightRow = (Type*)dst[1][v].ptr;
			
			Bool leftRaw, rightRaw;
			Type *thisRow, *lastRow, *nextRow;
			if (isLR)
			{
				if (v%2 == 0)
				{
					leftRaw = TRUE;
					rightRaw = FALSE;
					thisRow = (Type*)src[v].ptr;
					lastRow = (Type*)src[math::max(1, v-1)].ptr;
					nextRow = (Type*)src[v+1].ptr;
				}
				else
				{
					leftRaw = FALSE;
					rightRaw = TRUE;
					thisRow = (Type*)src[v].ptr;
					lastRow = (Type*)src[v-1].ptr;
					nextRow = (Type*)src[math::min((Int)height-2, v+1)].ptr;
				}
			}
			else		// RL
			{
				if (v%2 == 0)
				{
					leftRaw = FALSE;
					rightRaw = TRUE;
					thisRow = (Type*)src[v].ptr;
					lastRow = (Type*)src[math::max(1, v-1)].ptr;
					nextRow = (Type*)src[v+1].ptr;
				}
				else
				{
					leftRaw = TRUE;
					rightRaw = FALSE;
					thisRow = (Type*)src[v].ptr;
					lastRow = (Type*)src[v-1].ptr;
					nextRow = (Type*)src[math::min((Int)height-2, v+1)].ptr;
				}
			}
			
			if (leftRaw)
			{
				memoryCopy(thisRow, leftRow, rowBytes);
			}
			else
			{
				if (floating)
				{
					for (UInt u = 0; u < width; u++)
					{
						for (UInt n = 0; n < nChannels; n++)
						{
							leftRow[nChannels * u + n] = (Type)((lastRow[nChannels * u + n] + nextRow[nChannels * u + n]) * 0.5f);
						}
					}
				}
				else
				{
					for (UInt u = 0; u < width; u++)
					{
						for (UInt n = 0; n < nChannels; n++)
						{
							leftRow[nChannels * u + n] = (lastRow[nChannels * u + n] / 2) + (nextRow[nChannels * u + n] / 2);
						}
					}
				}
			}
			
			if (rightRaw)
			{
				memoryCopy(thisRow, rightRow, rowBytes);
			}
			else
			{
				if (floating)
				{
					for (UInt u = 0; u < width; u++)
					{
						for (UInt n = 0; n < nChannels; n++)
						{
							rightRow[nChannels * u + n] = (Type)((lastRow[nChannels * u + n] + nextRow[nChannels * u + n]) * 0.5f);
						}
					}
				}
				else
				{
					for (UInt u = 0; u < width; u++)
					{
						for (UInt n = 0; n < nChannels; n++)
						{
							rightRow[nChannels * u + n] = (lastRow[nChannels * u + n] / 2) + (nextRow[nChannels * u + n] / 2);
						}
					}
				}
			}
			// row end
		}
	}
	
}

void image_internal::mergeStereoByte(Image src[2], Image& dst, Enum<MergeMode> targetMergeMode, UInt nChannels)
{
    switch (targetMergeMode.value())
    {
        case MergeMode::FullSideBySide:
            mergeFSBS<Byte>(src, dst, nChannels);
            break;
        case MergeMode::HalfSideBySide:
            mergeHSBS<Byte>(src, dst, nChannels);
            break;
        case MergeMode::LineByLineLR:
            mergeLBL(src, dst, TRUE);
            break;
        case MergeMode::LineByLineRL:
            mergeLBL(src, dst, FALSE);
            break;
        default:
            break;
    }
}
void image_internal::mergeStereoWord(Image src[2], Image& dst, Enum<MergeMode> targetMergeMode, UInt nChannels)
{
    switch (targetMergeMode.value())
    {
        case MergeMode::FullSideBySide:
            mergeFSBS<Word>(src, dst, nChannels);
            break;
        case MergeMode::HalfSideBySide:
            mergeHSBS<Word>(src, dst, nChannels);
            break;
        case MergeMode::LineByLineLR:
            mergeLBL(src, dst, TRUE);
            break;
        case MergeMode::LineByLineRL:
            mergeLBL(src, dst, FALSE);
            break;
        default:
            break;
    }
}
void image_internal::splitStereoByte(Image src, Image dst[2], Enum<MergeMode> srcMergeMode, UInt nChannels)
{
    switch (srcMergeMode.value())
    {
        case MergeMode::HalfSideBySide:
            splitHSBS<Byte>(src, dst, nChannels);
            break;
        case MergeMode::FullSideBySide:
            splitFSBS<Byte>(src, dst);
            break;
        case MergeMode::LineByLineLR:
            splitLBL<Byte>(src, dst, nChannels, TRUE, FALSE);
            break;
        case MergeMode::LineByLineRL:
            splitLBL<Byte>(src, dst, nChannels, FALSE, FALSE);
            break;
        default:
            break;
    }
}
void image_internal::splitStereoWord(Image src, Image dst[2], Enum<MergeMode> srcMergeMode, UInt nChannels)
{
    switch (srcMergeMode.value())
    {
        case MergeMode::HalfSideBySide:
            splitHSBS<Word>(src, dst, nChannels);
            break;
        case MergeMode::FullSideBySide:
            splitFSBS<Word>(src, dst);
            break;
        case MergeMode::LineByLineLR:
            splitLBL<Word>(src, dst, nChannels, TRUE, FALSE);
            break;
        case MergeMode::LineByLineRL:
            splitLBL<Word>(src, dst, nChannels, FALSE, FALSE);
            break;
        default:
            break;
    }
}

void image_internal::mergeFieldGeneric(Image src[2], Image& dst)
{
	mergeLBL(src, dst, TRUE);
}

void image_internal::splitFieldByte(Image src, Image dst[2], UInt nChannels)
{
	splitLBL<Byte>(src, dst, nChannels, TRUE, FALSE);
}

void image_internal::splitFieldWord(Image src, Image dst[2], UInt nChannels)
{
	splitLBL<Word>(src, dst, nChannels, TRUE, FALSE);
}

void image_internal::splitFieldFloat(Image src, Image dst[2], UInt nChannels)
{
	splitLBL<Float>(src, dst, nChannels, TRUE, TRUE);
}