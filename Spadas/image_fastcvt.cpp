
#include "image.h"

using namespace image_internal;
using namespace spadas::math;

void image_internal::fastCvtByteBGR2ByteGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0; u < width; u++, u3 += 3)
		{
            dstRow[u] = (Byte)((table.BToY[(UInt)srcRow[u3] << 8] + table.GToY[(UInt)srcRow[u3+1] << 8] + table.RToY[(UInt)srcRow[u3+2] << 8]) >> 8);
		}
	}
}
void image_internal::fastCvtByteBGR2FloatBGR(Image src, Image& dst)
{
	Float k = 1.0f / 255;
	UInt width = src.width(), height = src.height();
	UInt width3 = width * 3;
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Float *dstRow = dst[v].f;
		for (UInt u = 0; u < width3; u++)
		{
			dstRow[u] = k * srcRow[u];
		}
	}
}
void image_internal::fastCvtByteBGR2FloatGray(Image src, Image& dst)
{
	Float k = 1.0f / 65535;
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Float *dstRow = dst[v].f;
		for (UInt u = 0, u3 = 0; u < width; u++, u3 += 3)
		{
			dstRow[u] = k * (table.BToY[(UInt)srcRow[u3] << 8] + table.GToY[(UInt)srcRow[u3+1] << 8] + table.RToY[(UInt)srcRow[u3+2] << 8]);
		}
	}
}
void image_internal::fastCvtByteBGR2WordBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	UInt width3 = width * 3;
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Word *dstRow = dst[v].w;
		for (UInt u = 0; u < width3; u++)
		{
			dstRow[u] = srcRow[u] << 8;
		}
	}
}
void image_internal::fastCvtByteBGR2WordGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Word *dstRow = dst[v].w;
		for (UInt u = 0, u3 = 0; u < width; u++, u3 += 3)
		{
			dstRow[u] = (Word)(table.BToY[(UInt)srcRow[u3] << 8] + table.GToY[(UInt)srcRow[u3+1] << 8] + table.RToY[(UInt)srcRow[u3+2] << 8]);
		}
	}
}
void image_internal::fastCvtByteBGR2ByteRGB(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u3b = 2; u < width; u++, u3b += 6)
		{
			dstRow[u3++] = srcRow[u3b--];
			dstRow[u3++] = srcRow[u3b--];
			dstRow[u3++] = srcRow[u3b--];
		}
	}
}
void image_internal::fastCvtByteBGR2ByteUYVY(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	UInt width2 = width / 2;
	Int R1, G1, B1, R2, G2, B2, R0, G0, B0;
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u4 = 0, u6 = 0; u < width2; u++)
		{
			B1 = (Int)srcRow[u6++] << 8;
			G1 = (Int)srcRow[u6++] << 8;
			R1 = (Int)srcRow[u6++] << 8;
			B2 = (Int)srcRow[u6++] << 8;
			G2 = (Int)srcRow[u6++] << 8;
			R2 = (Int)srcRow[u6++] << 8;
			B0 = (B1 + B2) >> 1;
			G0 = (G1 + G2) >> 1;
			R0 = (R1 + R2) >> 1;
			
			dstRow[u4++] = min(65535, max(0, table.RToU[R0] + table.GToU[G0] + (B0 >> 1) + 32768)) >> 8;
			dstRow[u4++] = (table.RToY[R1] + table.GToY[G1] + table.BToY[B1]) >> 8;
			dstRow[u4++] = min(65535, max(0, (R0 >> 1) + table.GToV[G0] + table.BToV[B0] + 32768)) >> 8;
			dstRow[u4++] = (table.RToY[R2] + table.GToY[G2] + table.BToY[B2]) >> 8;
		}
	}
}
void image_internal::fastCvtByteBGR2ByteYUV(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u3b = 0; u < width; u++)
		{
			Int B = (Int)srcRow[u3++] << 8;
			Int G = (Int)srcRow[u3++] << 8;
			Int R = (Int)srcRow[u3++] << 8;
			
			dstRow[u3b++] = (table.RToY[R] + table.GToY[G] + table.BToY[B]) >> 8;
			dstRow[u3b++] = min(65535, max(0, table.RToU[R] + table.GToU[G] + (B >> 1) + 32768)) >> 8;
			dstRow[u3b++] = min(65535, max(0, (R >> 1) + table.GToV[G] + table.BToV[B] + 32768)) >> 8;
		}
	}
}
void image_internal::fastCvtByteBGR2ByteBGRA(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u4 = 0; u < width; u++)
		{
			dstRow[u4++] = srcRow[u3++];
			dstRow[u4++] = srcRow[u3++];
			dstRow[u4++] = srcRow[u3++];
            dstRow[u4++] = 255;
		}
	}
}
void image_internal::fastCvtByteBGR2ByteRGBA(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u4 = 0, u3b = 2; u < width; u++, u3b += 6)
		{
			dstRow[u4++] = srcRow[u3b--];
			dstRow[u4++] = srcRow[u3b--];
			dstRow[u4++] = srcRow[u3b--];
            dstRow[u4++] = 255;
		}
	}
}

void image_internal::fastCvtByteGray2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0; u < width; u++)
		{
			Byte val = srcRow[u];
			dstRow[u3++] = val;
			dstRow[u3++] = val;
			dstRow[u3++] = val;
		}
	}
}
void image_internal::fastCvtByteGray2FloatGray(Image src, Image& dst)
{
	Float k = 1.0f / 255;
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Float *dstRow = dst[v].f;
		for (UInt u = 0; u < width; u++)
		{
			dstRow[u] = k * srcRow[u];
		}
	}
}
void image_internal::fastCvtByteGray2WordGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Word *dstRow = dst[v].w;
		for (UInt u = 0; u < width; u++)
		{
			dstRow[u] = (Word)srcRow[u] << 8;
		}
	}
}
void image_internal::fastCvtByteGray2ByteUYVY(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u2 = 0; u < width; u++)
		{
			dstRow[u2++] = 128;
			dstRow[u2++] = srcRow[u];
		}
	}
}
void image_internal::fastCvtByteGray2ByteYUV(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0; u < width; u++)
		{
			dstRow[u3++] = srcRow[u];
			dstRow[u3++] = 128;
			dstRow[u3++] = 128;
		}
	}
}

void image_internal::fastCvtFloatBGR2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	UInt width3 = width * 3;
	for (UInt v = 0; v < height; v++)
	{
		Float *srcRow = src[v].f;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0; u < width3; u++)
		{
			dstRow[u] = (Byte)(srcRow[u] * 255);
		}
	}
}

void image_internal::fastCvtFloatGray2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Float *srcRow = src[v].f;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0; u < width; u++)
		{
			Byte val = (Byte)(srcRow[u] * 255);
			dstRow[u3++] = val;
			dstRow[u3++] = val;
			dstRow[u3++] = val;
		}
	}
}
void image_internal::fastCvtFloatGray2ByteGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Float *srcRow = src[v].f;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0; u < width; u++)
		{
			dstRow[u]  = (Byte)(srcRow[u] * 255);
		}
	}
}

void image_internal::fastCvtFloatGray2WordGray(spadas::Image src, spadas::Image &dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Float *srcRow = src[v].f;
		Word *dstRow = dst[v].w;
		for (UInt u = 0; u < width; u++)
		{
			dstRow[u]  = (Word)(srcRow[u] * 65535);
		}
	}
}

void image_internal::fastCvtWordBGR2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	UInt width3 = width * 3;
	for (UInt v = 0; v < height; v++)
	{
		Word *srcRow = src[v].w;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0; u < width3; u++)
		{
			dstRow[u] = srcRow[u] >> 8;
		}
	}
}
void image_internal::fastCvtWordBGR2WordGray(spadas::Image src, spadas::Image &dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Word *srcRow = src[v].w;
		Word *dstRow = dst[v].w;
		for (UInt u = 0, u3 = 0; u < width; u++, u3 += 3)
		{
            dstRow[u] = table.BToY[srcRow[u3]] + table.GToY[srcRow[u3+1]] + table.RToY[srcRow[u3+2]];
		}
	}
}

void image_internal::fastCvtWordGray2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Word *srcRow = src[v].w;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0; u < width; u++)
		{
			Byte val = srcRow[u] >> 8;
			dstRow[u3++] = val;
			dstRow[u3++] = val;
			dstRow[u3++] = val;
		}
	}
}
void image_internal::fastCvtWordGray2ByteGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Word *srcRow = src[v].w;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0; u < width; u++)
		{
			dstRow[u] = srcRow[u] >> 8;
		}
	}
}

void image_internal::fastCvtByteRGB2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u3b = 2; u < width; u++, u3b += 6)
		{
			dstRow[u3++] = srcRow[u3b--];
			dstRow[u3++] = srcRow[u3b--];
			dstRow[u3++] = srcRow[u3b--];
		}
	}
}
void image_internal::fastCvtByteRGB2ByteRGBA(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u4 = 0; u < width; u++)
		{
			dstRow[u4++] = srcRow[u3++];
			dstRow[u4++] = srcRow[u3++];
			dstRow[u4++] = srcRow[u3++];
            dstRow[u4++] = 255;
		}
	}
}

void image_internal::fastCvtByteUYVY2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	UInt width2 = width / 2;
	Int uToB, uvToG, vToR;
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u4 = 0, u6 = 0; u < width2; u++)
		{
			Int U = (Int)srcRow[u4++] << 8;
			Int Y1 = (Int)srcRow[u4++] << 8;
			Int V = (Int)srcRow[u4++] << 8;
			Int Y2 = (Int)srcRow[u4++] << 8;
			
			uToB = table.UToB[U];
			uvToG = table.UToG[U] + table.VToG[V];
			vToR = table.VToR[V];
			
			dstRow[u6++] = min(65535, max(0, Y1 + uToB)) >> 8;
			dstRow[u6++] = min(65535, max(0, Y1 + uvToG)) >> 8;
			dstRow[u6++] = min(65535, max(0, Y1 + vToR)) >> 8;
			dstRow[u6++] = min(65535, max(0, Y2 + uToB)) >> 8;
			dstRow[u6++] = min(65535, max(0, Y2 + uvToG)) >> 8;
			dstRow[u6++] = min(65535, max(0, Y2 + vToR)) >> 8;
		}
	}
}
void image_internal::fastCvtByteUYVY2ByteGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u2 = 1; u < width; u++, u2 += 2)
		{
			dstRow[u] = srcRow[u2];
		}
	}
}

void image_internal::fastCvtByteYUV2ByteBGR(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	Int uToB, uvToG, vToR;
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u3b = 0; u < width; u++)
		{
			Int Y = (Int)srcRow[u3b++] << 8;
			Int U = (Int)srcRow[u3b++] << 8;
			Int V = (Int)srcRow[u3b++] << 8;
			
			uToB = table.UToB[U];
			uvToG = table.UToG[U] + table.VToG[V];
			vToR = table.VToR[V];
			
			dstRow[u3++] = min(65535, max(0, Y + uToB)) >> 8;
			dstRow[u3++] = min(65535, max(0, Y + uvToG)) >> 8;
			dstRow[u3++] = min(65535, max(0, Y + vToR)) >> 8;
		}
	}
}
void image_internal::fastCvtByteYUV2ByteGray(Image src, Image& dst)
{
	UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0; u < width; u++, u3 += 3)
		{
			dstRow[u] = srcRow[u3];
		}
	}
}

void image_internal::fastCvtByteBGRA2ByteBGR(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u4 = 0; u < width; u++, u4++)
		{
            dstRow[u3++] = srcRow[u4++];
            dstRow[u3++] = srcRow[u4++];
            dstRow[u3++] = srcRow[u4++];
		}
	}
}
void image_internal::fastCvtByteBGRA2ByteRGBA(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0; u < width; u++)
		{
            dstRow[4*u] = srcRow[4*u+2];
            dstRow[4*u+1] = srcRow[4*u+1];
            dstRow[4*u+2] = srcRow[4*u];
            dstRow[4*u+3] = srcRow[4*u+3];
		}
	}
}

void image_internal::fastCvtByteRGBA2ByteBGR(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
    for (UInt v = 0; v < height; v++)
    {
        Byte *srcRow = src[v].b;
        Byte *dstRow = dst[v].b;
        for (UInt u = 0, u4 = 0, u3b = 2; u < width; u++, u3b += 6)
        {
            dstRow[u3b--] = srcRow[u4++];
            dstRow[u3b--] = srcRow[u4++];
            dstRow[u3b--] = srcRow[u4++];
            u4++;
        }
    }
}
void image_internal::fastCvtByteRGBA2ByteRGB(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0, u3 = 0, u4 = 0; u < width; u++, u4++)
		{
            dstRow[u3++] = srcRow[u4++];
            dstRow[u3++] = srcRow[u4++];
            dstRow[u3++] = srcRow[u4++];
		}
	}
}
void image_internal::fastCvtByteRGBA2ByteBGRA(Image src, Image& dst)
{
    UInt width = src.width(), height = src.height();
	for (UInt v = 0; v < height; v++)
	{
		Byte *srcRow = src[v].b;
		Byte *dstRow = dst[v].b;
		for (UInt u = 0; u < width; u++)
		{
            dstRow[4*u] = srcRow[4*u+2];
            dstRow[4*u+1] = srcRow[4*u+1];
            dstRow[4*u+2] = srcRow[4*u];
            dstRow[4*u+3] = srcRow[4*u+3];
		}
	}
}