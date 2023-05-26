
#include "image.h"

using namespace image_internal;
using namespace spadas::math;
using namespace spadas::utility;

///////////////////////////////////////////////
// resize(Size2D) in ResizeMode::Area
template<typename Type>
void areaResizeColor(Image src, Image& dst, Float widthRatio, Float heightRatio, UInt nChannels)
{
	UInt srcWidth = src.width(), srcHeight = src.height();
	UInt dstWidth = dst.width(), dstHeight = dst.height();
	Float srcCenterU = (Float)(srcWidth-1) / 2, srcCenterV = (Float)(srcHeight-1) / 2;
	Float dstCenterU = (Float)(dstWidth-1) / 2, dstCenterV = (Float)(dstHeight-1) / 2;

    Float *intensitySum = new Float[nChannels];
	for (UInt v = 0; v < dstHeight; v++)
	{
		Type *dstRow = (Type*)dst[v].ptr;
		for (UInt u = 0; u < dstWidth; u++)
		{
			Float dstUNorm = (Float)u - dstCenterU;
			Float dstVNorm = (Float)v - dstCenterV;
			Float srcUMin = (dstUNorm - 0.5f) * widthRatio + srcCenterU;
			Float srcUMax = srcUMin + widthRatio;
			Float srcVMin = (dstVNorm - 0.5f) * heightRatio + srcCenterV;
			Float srcVMax = srcVMin + heightRatio;
			Int srcUMinI = math::floor(srcUMin + 0.501f);
			Int srcUMaxI = math::ceil(srcUMax - 0.501f);
			Int srcVMinI = math::floor(srcVMin + 0.501f);
			Int srcVMaxI = math::ceil(srcVMax - 0.501f);
			Float weightSum = 0.0f;
            for (UInt n = 0; n < nChannels; n++) intensitySum[n] = 0.0f;
			
			for (Int m = srcVMinI; m <= srcVMaxI; m++)
			{
				Float cellVMin = max((Float)m - 0.5f, srcVMin);
				Float cellVMax = min((Float)m + 0.5f, srcVMax);
				Float cellVCenter = (cellVMax + cellVMin) / 2;
				Int assistV = 0;
				Float pv = 0.0f;
				if (cellVCenter > m)
				{
					assistV = clamp(m+1, 0, (Int)srcHeight-1);
					pv = cellVCenter - m;
				}
				else
				{
					assistV = clamp(m-1, 0, (Int)srcHeight-1);
					pv = m - cellVCenter;
				}
				
				Type *mainRow = (Type*)src[m].ptr;
				Type *assistRow = (Type*)src[assistV].ptr;
				for (Int n = srcUMinI; n <= srcUMaxI; n++)
				{
					Float cellUMin = max((Float)n - 0.5f, srcUMin);
					Float cellUMax = min((Float)n + 0.5f, srcUMax);
					Float cellUCenter = (cellUMax + cellUMin) / 2;
					Int assistU = 0;
					Float pu = 0.0f;
					if (cellUCenter > n)
					{
						assistU = clamp(n+1, 0, (Int)srcWidth-1);
						pu = cellUCenter - n;
					}
					else
					{
						assistU = clamp(n-1, 0, (Int)srcWidth-1);
						pu = n - cellUCenter;
					}
					
					Float weight = (cellUMax - cellUMin) * (cellVMax - cellVMin);
                    for (UInt x = 0; x < nChannels; x++)
                    {
                        Float intensity = (1.0f - pu) * ((1.0f - pv) * mainRow[nChannels*n+x] + pv * assistRow[nChannels*n+x]) + pu * (pv * assistRow[nChannels*assistU+x] + (1.0f - pv) * mainRow[nChannels*assistU+x]);
                        intensitySum[x] += intensity * weight;
                    }
					weightSum += weight;
				}
			}

			Float weightSum1 = 1.0f / weightSum;
            for (UInt x = 0; x < nChannels; x++)
            {
                dstRow[nChannels*u+x] = (Type)(intensitySum[x] * weightSum1);
            }
		}
	}
    delete[] intensitySum;
}

template<typename Type>
void areaResizeGray(Image src, Image& dst, Float widthRatio, Float heightRatio)
{
	UInt srcWidth = src.width(), srcHeight = src.height();
	UInt dstWidth = dst.width(), dstHeight = dst.height();
	Float srcCenterU = (Float)(srcWidth-1) / 2, srcCenterV = (Float)(srcHeight-1) / 2;
	Float dstCenterU = (Float)(dstWidth-1) / 2, dstCenterV = (Float)(dstHeight-1) / 2;
	
	for (UInt v = 0; v < dstHeight; v++)
	{
		Type *dstRow = (Type*)dst[v].ptr;
		for (UInt u = 0; u < dstWidth; u++)
		{
			Float dstUNorm = (Float)u - dstCenterU;
			Float dstVNorm = (Float)v - dstCenterV;
			Float srcUMin = (dstUNorm - 0.5f) * widthRatio + srcCenterU;
			Float srcUMax = srcUMin + widthRatio;
			Float srcVMin = (dstVNorm - 0.5f) * heightRatio + srcCenterV;
			Float srcVMax = srcVMin + heightRatio;
			Int srcUMinI = math::floor(srcUMin + 0.501f);
			Int srcUMaxI = math::ceil(srcUMax - 0.501f);
			Int srcVMinI = math::floor(srcVMin + 0.501f);
			Int srcVMaxI = math::ceil(srcVMax - 0.501f);
			Float intensitySum = 0.0f;
			Float weightSum = 0.0f;
			
			for (Int m = srcVMinI; m <= srcVMaxI; m++)
			{
				Float cellVMin = max((Float)m - 0.5f, srcVMin);
				Float cellVMax = min((Float)m + 0.5f, srcVMax);
				Float cellVCenter = (cellVMax + cellVMin) / 2;
				Int assistV = 0;
				Float pv = 0.0f;
				if (cellVCenter > m)
				{
					assistV = clamp(m+1, 0, (Int)srcHeight-1);
					pv = cellVCenter - m;
				}
				else
				{
					assistV = clamp(m-1, 0, (Int)srcHeight-1);
					pv = m - cellVCenter;
				}
				
				Type *mainRow = (Type*)src[m].ptr;
				Type *assistRow = (Type*)src[assistV].ptr;
				for (Int n = srcUMinI; n <= srcUMaxI; n++)
				{
					Float cellUMin = max((Float)n - 0.5f, srcUMin);
					Float cellUMax = min((Float)n + 0.5f, srcUMax);
					Float cellUCenter = (cellUMax + cellUMin) / 2;
					Int assistU = 0;
					Float pu = 0.0f;
					if (cellUCenter > n)
					{
						assistU = clamp(n+1, 0, (Int)srcWidth-1);
						pu = cellUCenter - n;
					}
					else
					{
						assistU = clamp(n-1, 0, (Int)srcWidth-1);
						pu = n - cellUCenter;
					}
					
					Float weight = (cellUMax - cellUMin) * (cellVMax - cellVMin);
					
					intensitySum += ((1.0f - pu) * ((1.0f - pv) * mainRow[n] + pv * assistRow[n]) + pu * (pv * assistRow[assistU] + (1.0f - pv) * mainRow[assistU])) * weight;
					weightSum += weight;
				}
			}

			dstRow[u] = (Type)(intensitySum / weightSum);
		}
	}
}

void image_internal::areaResizeColorByte(Image src, Image& dst, Float widthRatio, Float heightRatio, UInt nChannels)
{
	areaResizeColor<Byte>(src, dst, widthRatio, heightRatio, nChannels);
}
void image_internal::areaResizeColorWord(Image src, Image& dst, Float widthRatio, Float heightRatio)
{
	areaResizeColor<Word>(src, dst, widthRatio, heightRatio, 3);
}
void image_internal::areaResizeColorFloat(Image src, Image& dst, Float widthRatio, Float heightRatio)
{
	areaResizeColor<Float>(src, dst, widthRatio, heightRatio, 3);
}
void image_internal::areaResizeGrayByte(Image src, Image& dst, Float widthRatio, Float heightRatio)
{
	areaResizeGray<Byte>(src, dst, widthRatio, heightRatio);
}
void image_internal::areaResizeGrayWord(Image src, Image& dst, Float widthRatio, Float heightRatio)
{
	areaResizeGray<Word>(src, dst, widthRatio, heightRatio);
}
void image_internal::areaResizeGrayFloat(Image src, Image& dst, Float widthRatio, Float heightRatio)
{
	areaResizeGray<Float>(src, dst, widthRatio, heightRatio);
}

///////////////////////////////////////////////
// resize(ResizeScale)
template <typename Type>
void resizeColorWithScale(Image src, Image& dst, Enum<ResizeScale> scale, UInt nChannels)
{
	if (scale == ResizeScale::Value::Quarter)
	{
		UInt width = src.width() / 4, height = src.height() / 4;
        UInt uSrcStep = 3 * nChannels;
		for (UInt v = 0; v < height; v++)
		{
			Type *srcRow1 = (Type*)src[4*v].ptr;
			Type *srcRow2 = (Type*)src[4*v+1].ptr;
			Type *srcRow3 = (Type*)src[4*v+2].ptr;
			Type *srcRow4 = (Type*)src[4*v+3].ptr;
			Type *dstRow = (Type*)dst[v].ptr;
			for (UInt u = 0, uDst = 0, uSrc = 0; u < width; u++, uSrc += uSrcStep)
			{
                for (UInt n = 0; n < nChannels; n++)
                {
                    dstRow[uDst++] = ((Int)srcRow1[uSrc] + (Int)srcRow1[uSrc+nChannels] + (Int)srcRow1[uSrc+nChannels*2] + (Int)srcRow1[uSrc+nChannels*3] +
                                    (Int)srcRow2[uSrc] + (Int)srcRow2[uSrc+nChannels] + (Int)srcRow2[uSrc+nChannels*2] + (Int)srcRow2[uSrc+nChannels*3] +
                                    (Int)srcRow3[uSrc] + (Int)srcRow3[uSrc+nChannels] + (Int)srcRow3[uSrc+nChannels*2] + (Int)srcRow3[uSrc+nChannels*3] +
                                    (Int)srcRow4[uSrc] + (Int)srcRow4[uSrc+nChannels] + (Int)srcRow4[uSrc+nChannels*2] + (Int)srcRow4[uSrc+nChannels*3]) >> 4;
                    uSrc++;
                }
			}
		}
	}
	
	if (scale == ResizeScale::Value::Half)
	{
		UInt width = src.width() / 2, height = src.height() / 2;
		for (UInt v = 0; v < height; v++)
		{
			Type *srcRow1 = (Type*)src[2*v].ptr;
			Type *srcRow2 = (Type*)src[2*v+1].ptr;
			Type *dstRow = (Type*)dst[v].ptr;
			for (UInt u = 0, uDst = 0, uSrc = 0; u < width; u++, uSrc += nChannels)
			{
                for (UInt n = 0; n < nChannels; n++)
                {
                    dstRow[uDst++] = ((Int)srcRow1[uSrc] + (Int)srcRow1[uSrc+nChannels] + (Int)srcRow2[uSrc] + (Int)srcRow2[uSrc+nChannels]) >> 2;
                    uSrc++;
                }
			}
		}
	}
	
	if (scale == ResizeScale::Value::Double)
	{
		UInt width = src.width(), height = src.height();
		
		Type *srcFirstRow = (Type*)src[0].ptr;
		Type *srcLastRow = (Type*)src[height-1].ptr;
		Type *dstFirstRow = (Type*)dst[0].ptr;
		Type *dstLastRow = (Type*)dst[2*height-1].ptr;
        
        for (UInt n = 0; n < nChannels; n++)
        {
            dstFirstRow[n] = srcFirstRow[n];
            dstLastRow[n] = srcLastRow[n];
            dstFirstRow[(2*width-1) * nChannels + n] = srcFirstRow[(width-1) * nChannels + n];
            dstLastRow[(2*width-1) * nChannels + n] = srcLastRow[(width-1) * nChannels + n];
        }

		for (UInt u = 1; u < width; u++)
		{
            for (UInt n = 0; n < nChannels; n++)
            {
                dstFirstRow[(2*u-1) * nChannels + n] = (srcFirstRow[(u-1) * nChannels + n] * 3 + srcFirstRow[u * nChannels + n]) >> 2;
                dstFirstRow[2*u * nChannels + n] = (srcFirstRow[(u-1) * nChannels + n] + srcFirstRow[u * nChannels + n] * 3) >> 2;
                dstLastRow[(2*u-1) * nChannels + n] = (srcLastRow[(u-1) * nChannels + n] * 3 + srcLastRow[u * nChannels + n]) >> 2;
                dstLastRow[2*u * nChannels + n] = (srcLastRow[(u-1) * nChannels + n] + srcLastRow[u * nChannels + n] * 3) >> 2;
            }
		}
		
		for (UInt v = 1; v < height; v++)
		{
			Type *srcRow1 = (Type*)src[v-1].ptr;
			Type *srcRow2 = (Type*)src[v].ptr;
			Type *dstRow1 = (Type*)dst[2*v-1].ptr;
			Type *dstRow2 = (Type*)dst[2*v].ptr;
            
            UInt lastIndexN = nChannels * (width - 1);
			UInt lastIndex2N = nChannels * (2 * width - 1);
            
            for (UInt n = 0; n < nChannels; n++)
            {
                dstRow1[n] = ((Int)srcRow1[n] * 3 + (Int)srcRow2[n]) >> 2;
                dstRow2[n] = ((Int)srcRow1[n] + (Int)srcRow2[n] * 3) >> 2;
                dstRow1[lastIndex2N + n] = ((Int)srcRow1[lastIndexN + n] * 3 + (Int)srcRow2[lastIndexN + n]) >> 2;
                dstRow2[lastIndex2N + n] = ((Int)srcRow1[lastIndexN + n] + (Int)srcRow2[lastIndexN + n] * 3) >> 2;
            }
			
			for (UInt u = 0, uSrc = 0, uDst = nChannels; u < width-1; u++, uDst += nChannels)
			{
                for (UInt n = 0; n < nChannels; n++)
                {
                    dstRow1[uDst] = ((Int)srcRow1[uSrc] * 9 + (Int)srcRow1[uSrc+nChannels] * 3 + (Int)srcRow2[uSrc] * 3 + (Int)srcRow2[uSrc+nChannels]) >> 4;
                    dstRow1[uDst + nChannels] = ((Int)srcRow1[uSrc] * 3 + (Int)srcRow1[uSrc+nChannels] * 9 + (Int)srcRow2[uSrc] + (Int)srcRow2[uSrc+nChannels] * 3) >> 4;
                    dstRow2[uDst] = ((Int)srcRow1[uSrc] * 3 + (Int)srcRow1[uSrc+nChannels] + (Int)srcRow2[uSrc] * 9 + (Int)srcRow2[uSrc+nChannels] * 3) >> 4;
                    dstRow2[uDst + nChannels] = ((Int)srcRow1[uSrc] + (Int)srcRow1[uSrc+nChannels] * 3 + (Int)srcRow2[uSrc] * 3 + (Int)srcRow2[uSrc+nChannels] * 9) >> 4;
                    uSrc++;
                    uDst++;
                }
			}
		}
	}
}

template <typename Type>
void resizeGrayWithScale(Image src, Image& dst, Enum<ResizeScale> scale)
{
	if (scale == ResizeScale::Value::Quarter)
	{
		UInt width = src.width() / 4, height = src.height() / 4;
		for (UInt v = 0; v < height; v++)
		{
			Type *srcRow1 = (Type*)src[4*v].ptr;
			Type *srcRow2 = (Type*)src[4*v+1].ptr;
			Type *srcRow3 = (Type*)src[4*v+2].ptr;
			Type *srcRow4 = (Type*)src[4*v+3].ptr;
			Type *dstRow = (Type*)dst[v].ptr;
			for (UInt u = 0, u4 = 0; u < width; u++, u4 += 4)
			{
				dstRow[u] = ((Int)srcRow1[u4] + (Int)srcRow1[u4+1] + (Int)srcRow1[u4+2] + (Int)srcRow1[u4+3] +
							 (Int)srcRow2[u4] + (Int)srcRow2[u4+1] + (Int)srcRow2[u4+2] + (Int)srcRow2[u4+3] +
							 (Int)srcRow3[u4] + (Int)srcRow3[u4+1] + (Int)srcRow3[u4+2] + (Int)srcRow3[u4+3] +
							 (Int)srcRow4[u4] + (Int)srcRow4[u4+1] + (Int)srcRow4[u4+2] + (Int)srcRow4[u4+3]) >> 4;
			}
		}
	}
	
	if (scale == ResizeScale::Value::Half)
	{
		UInt width = src.width() / 2, height = src.height() / 2;
		for (UInt v = 0; v < height; v++)
		{
			Type *srcRow1 = (Type*)src[2*v].ptr;
			Type *srcRow2 = (Type*)src[2*v+1].ptr;
			Type *dstRow = (Type*)dst[v].ptr;
			for (UInt u = 0, u2 = 0; u < width; u++, u2 += 2)
			{
				dstRow[u] = ((Int)srcRow1[u2] + (Int)srcRow1[u2+1] + (Int)srcRow2[u2] + (Int)srcRow2[u2+1]) >> 2;
			}
		}
	}
	
	if (scale == ResizeScale::Value::Double)
	{
		UInt width = src.width(), height = src.height();
		
		Type *srcFirstRow = (Type*)src[0].ptr;
		Type *srcLastRow = (Type*)src[height-1].ptr;
		Type *dstFirstRow = (Type*)dst[0].ptr;
		Type *dstLastRow = (Type*)dst[2*height-1].ptr;
		dstFirstRow[0] = srcFirstRow[0];
		dstFirstRow[2*width-1] = srcFirstRow[width-1];
		dstLastRow[0] = srcLastRow[0];
		dstLastRow[2*width-1] = srcLastRow[width-1];
		for (UInt u = 1; u < width; u++)
		{
			dstFirstRow[2*u-1] = (srcFirstRow[u-1] * 3 + srcFirstRow[u]) >> 2;
			dstFirstRow[2*u] = (srcFirstRow[u-1] + srcFirstRow[u] * 3) >> 2;
			
			dstLastRow[2*u-1] = (srcLastRow[u-1] * 3 + srcLastRow[u]) >> 2;
			dstLastRow[2*u] = (srcLastRow[u-1] + srcLastRow[u] * 3) >> 2;
		}
		
		for (UInt v = 1; v < height; v++)
		{
			Type *srcRow1 = (Type*)src[v-1].ptr;
			Type *srcRow2 = (Type*)src[v].ptr;
			Type *dstRow1 = (Type*)dst[2*v-1].ptr;
			Type *dstRow2 = (Type*)dst[2*v].ptr;
			
			dstRow1[0] = ((Int)srcRow1[0] * 3 + (Int)srcRow2[0]) >> 2;
			dstRow2[0] = ((Int)srcRow1[0] + (Int)srcRow2[0] * 3) >> 2;
			
			dstRow1[2*width-1] = ((Int)srcRow1[width-1] * 3 + (Int)srcRow2[width-1]) >> 2;
			dstRow2[2*width-1] = ((Int)srcRow1[width-1] + (Int)srcRow2[width-1] * 3) >> 2;
			
			for (UInt u = 0, u2 = 1; u < width-1; u++, u2 += 2)
			{
				dstRow1[u2] = ((Int)srcRow1[u] * 9 + (Int)srcRow1[u+1] * 3 + (Int)srcRow2[u] * 3 + (Int)srcRow2[u+1]) >> 4;
				dstRow1[u2 + 1] = ((Int)srcRow1[u] * 3 + (Int)srcRow1[u+1] * 9 + (Int)srcRow2[u] + (Int)srcRow2[u+1] * 3) >> 4;
				dstRow2[u2] = ((Int)srcRow1[u] * 3 + (Int)srcRow1[u+1] + (Int)srcRow2[u] * 9 + (Int)srcRow2[u+1] * 3) >> 4;
				dstRow2[u2 + 1] = ((Int)srcRow1[u] + (Int)srcRow1[u+1] * 3 + (Int)srcRow2[u] * 3 + (Int)srcRow2[u+1] * 9) >> 4;
			}
		}
	}
}

void image_internal::resizeColorByteWithScale(Image src, Image& dst, Enum<ResizeScale> scale, UInt nChannels)
{
	resizeColorWithScale<Byte>(src, dst, scale, nChannels);
}

void image_internal::resizeGrayByteWithScale(Image src, Image& dst, Enum<ResizeScale> scale)
{
	resizeGrayWithScale<Byte>(src, dst, scale);
}

void image_internal::resizeColorWordWithScale(Image src, Image& dst, Enum<ResizeScale> scale)
{
	resizeColorWithScale<Word>(src, dst, scale, 3);
}

void image_internal::resizeGrayWordWithScale(Image src, Image& dst, Enum<ResizeScale> scale)
{
	resizeGrayWithScale<Word>(src, dst, scale);
}