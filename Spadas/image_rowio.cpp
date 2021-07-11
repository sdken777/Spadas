
#include "spadas.h"

#include "image_bz.h"

namespace image_internal
{
	using namespace spadas;
	const Float WORD2FLOAT = 0.000015259022f;
}

using namespace image_internal;
using namespace spadas::math;

void LoadByteBGR::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		b[u] = ptr[i++] << 8;
		g[u] = ptr[i++] << 8;
		r[u] = ptr[i++] << 8;
        a[u] = 255;
	}
}

void WriteByteBGR::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		ptr[i++] = b[u] >> 8;
		ptr[i++] = g[u] >> 8;
		ptr[i++] = r[u] >> 8;
	}
}

void LoadByteGray::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		val[u] = ptr[u] << 8;
	}
}

void WriteByteGray::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		ptr[u] = val[u] >> 8;
	}
}

void LoadWordBGR::io(Pointer row, Pointer data, UInt width)
{
	Word *ptr = (Word*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		b[u] = ptr[i++];
		g[u] = ptr[i++];
		r[u] = ptr[i++];
        a[u] = 255;
	}
}

void WriteWordBGR::io(Pointer row, Pointer data, UInt width)
{
	Word *ptr = (Word*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		ptr[i++] = b[u];
		ptr[i++] = g[u];
		ptr[i++] = r[u];
	}
}

void LoadWordGray::io(Pointer row, Pointer data, UInt width)
{
	Word *ptr = (Word*)row;
	
	Word **data0 = (Word**)data;
	Word *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		val[u] = ptr[u];
	}
}

void WriteWordGray::io(Pointer row, Pointer data, UInt width)
{
	Word *ptr = (Word*)row;
	
	Word **data0 = (Word**)data;
	Word *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		ptr[u] = val[u];
	}
}

void LoadFloatBGR::io(Pointer row, Pointer data, UInt width)
{
	Float *ptr = (Float*)row;
	
	Float **data0 = (Float**)data;
	Float *r = data0[0];
	Float *g = data0[1];
	Float *b = data0[2];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		b[u] = clamp(ptr[i++], 0.0f, 1.0f);
		g[u] = clamp(ptr[i++], 0.0f, 1.0f);
		r[u] = clamp(ptr[i++], 0.0f, 1.0f);
	}
}

void WriteFloatBGR::io(Pointer row, Pointer data, UInt width)
{
	Float *ptr = (Float*)row;
	
	Float **data0 = (Float**)data;
	Float *r = data0[0];
	Float *g = data0[1];
	Float *b = data0[2];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		ptr[i++] = b[u];
		ptr[i++] = g[u];
		ptr[i++] = r[u];
	}
}

void LoadFloatGray::io(Pointer row, Pointer data, UInt width)
{
	Float *ptr = (Float*)row;
	
	Float **data0 = (Float**)data;
	Float *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		val[u] = clamp(ptr[u], 0.0f, 1.0f);
	}
}

void WriteFloatGray::io(Pointer row, Pointer data, UInt width)
{
	Float *ptr = (Float*)row;
	
	Float **data0 = (Float**)data;
	Float *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		ptr[u] = val[u];
	}
}

void LoadByteRGB::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		r[u] = ptr[i++] << 8;
		g[u] = ptr[i++] << 8;
		b[u] = ptr[i++] << 8;
        a[u] = 255;
	}
}

void WriteByteRGB::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		ptr[i++] = r[u] >> 8;
		ptr[i++] = g[u] >> 8;
		ptr[i++] = b[u] >> 8;
	}
}

void LoadByteUYVY::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	Word y1, y2, cb, cr;
	for (UInt u = 0, u2 = 0; u2 < width/2; u += 2, u2++)
	{
		cb = ptr[i++] << 8;
		y1 = ptr[i++] << 8;
		cr = ptr[i++] << 8;
		y2 = ptr[i++] << 8;
		
		r[u] = (Word)clamp((Int)y1 + table.VToR[cr], 0, 65535);
		g[u] = (Word)clamp((Int)y1 + table.UToG[cb] + table.VToG[cr], 0, 65535);
		b[u] = (Word)clamp((Int)y1 + table.UToB[cb], 0, 65535);
        a[u] = 255;
		
		r[u+1] = (Word)clamp((Int)y2 + table.VToR[cr], 0, 65535);
		g[u+1] = (Word)clamp((Int)y2 + table.UToG[cb] + table.VToG[cr], 0, 65535);
		b[u+1] = (Word)clamp((Int)y2 + table.UToB[cb], 0, 65535);
        a[u+1] = 255;
	}
}

void WriteByteUYVY::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
	
	UInt i = 0;
	Word r0, g0, b0, r1, g1, b1, r2, g2, b2;
	Word y1, y2, cb, cr;
	for (UInt u = 0, u2 = 0; u2 < width/2; u += 2, u2++)
	{
		r1 = r[u];
		g1 = g[u];
		b1 = b[u];
		r2 = r[u+1];
		g2 = g[u+1];
		b2 = b[u+1];
		r0 = (r1 >> 1) + (r2 >> 1);
		g0 = (g1 >> 1) + (g2 >> 1);
		b0 = (b1 >> 1) + (b2 >> 1);
		
		y1 = (Word)clamp(table.RToY[r1] + table.GToY[g1] + table.BToY[b1], 0, 65535);
		y2 = (Word)clamp(table.RToY[r2] + table.GToY[g2] + table.BToY[b2], 0, 65535);
		cb = (Word)clamp(table.RToU[r0] + table.GToU[g0] + table.BToU[b0] + 32768, 0, 65535);
		cr = (Word)clamp(table.RToV[r0] + table.GToV[g0] + table.BToV[b0] + 32768, 0, 65535);
		
		ptr[i++] = (Byte)(cb >> 8);
		ptr[i++] = (Byte)(y1 >> 8);
		ptr[i++] = (Byte)(cr >> 8);
		ptr[i++] = (Byte)(y2 >> 8);
	}
}

void LoadByteYUV::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	Word y, cb, cr;
	for (UInt u = 0; u < width; u++)
	{
		y = ptr[i++] << 8;
		cb = ptr[i++] << 8;
		cr = ptr[i++] << 8;
		
		r[u] = (Word)clamp((Int)y + table.VToR[cr], 0, 65535);
		g[u] = (Word)clamp((Int)y + table.UToG[cb] + table.VToG[cr], 0, 65535);
		b[u] = (Word)clamp((Int)y + table.UToB[cb], 0, 65535);
        a[u] = 255;
	}
}

void WriteByteYUV::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
	
	UInt i = 0;
	Word r0, g0, b0;
	Word y, cb, cr;
	for (UInt u = 0; u < width; u++)
	{
		r0 = r[u];
		g0 = g[u];
		b0 = b[u];
		
		y = (Word)clamp(table.RToY[r0] + table.GToY[g0] + table.BToY[b0], 0, 65535);
		cb = (Word)clamp(table.RToU[r0] + table.GToU[g0] + table.BToU[b0] + 32768, 0, 65535);
		cr = (Word)clamp(table.RToV[r0] + table.GToV[g0] + table.BToV[b0] + 32768, 0, 65535);
		
		ptr[i++] = (Byte)(y >> 8);
		ptr[i++] = (Byte)(cb >> 8);
		ptr[i++] = (Byte)(cr >> 8);
	}
}

void LoadFloatHSV::io(Pointer row, Pointer data, UInt width)
{
	Float *ptr = (Float*)row;
	
	Float **data0 = (Float**)data;
	Float *r = data0[0];
	Float *g = data0[1];
	Float *b = data0[2];
	
	Float h, s, v, h60, f, p, q, t;
	UInt hi;
	Float k = 1.0f / 60;
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		h = clamp(ptr[i++], 0.00001f, 359.99999f);
		s = clamp(ptr[i++], 0.0f, 1.0f);
		v = clamp(ptr[i++], 0.0f, 1.0f);
		
		h60 = h * k;
		hi = (UInt)floor(h60);
		f = h60 - hi;
		p = v * (1.0f - s);
		q = v * (1.0f - f * s);
		t = v * (1.0f - (1.0f - f) * s);
		
		switch (hi)
		{
			case 0:
				r[u] = v;
				g[u] = t;
				b[u] = p;
				break;
			case 1:
				r[u] = q;
				g[u] = v;
				b[u] = p;
				break;
			case 2:
				r[u] = p;
				g[u] = v;
				b[u] = t;
				break;
			case 3:
				r[u] = p;
				g[u] = q;
				b[u] = v;
				break;
			case 4:
				r[u] = t;
				g[u] = p;
				b[u] = v;
				break;
			case 5:
				r[u] = v;
				g[u] = p;
				b[u] = q;
				break;
		}
	}
}

void WriteFloatHSV::io(Pointer row, Pointer data, UInt width)
{
	Float *ptr = (Float*)row;
	
	Float **data0 = (Float**)data;
	Float *r = data0[0];
	Float *g = data0[1];
	Float *b = data0[2];
	
	Float r0, g0, b0, minv, maxv, d, d60;
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		r0 = r[u];
		g0 = g[u];
		b0 = b[u];
		
		minv = min(r0, min(g0, b0));
		maxv = max(r0, max(g0, b0));
		
		if ((d = maxv - minv) == 0.0f)
		{
			ptr[i++] = 0.0f;
			ptr[i++] = 0.0f;
			ptr[i++] = maxv;
			
			continue;
		}
		
		d60 = 60.0f / d;
		
		if (maxv == r0)
		{
			ptr[i] = d60 * (g0 - b0);
			ptr[i] += ptr[i] < 0.0f ? 360.0f : 0.0f;
			i++;
		}
		else if (maxv == g0)
		{
			ptr[i++] = d60 * (b0 - r0) + 120.0f;
		}
		else /* maxv == b0 */
		{
			ptr[i++] = d60 * (r0 - g0) + 240.0f;
		}
		
		ptr[i++] = d / maxv;
		ptr[i++] = maxv;
	}
}

void LoadByteBool::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		val[u] = ptr[u] < 128 ? 0 : 65535;
	}
}

void WriteByteBool::io(Pointer row, Pointer data, UInt width)
{
	Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *val = data0[0];
	
	for (UInt u = 0; u < width; u++)
	{
		ptr[u] = val[u] < 32768 ? 0 : 255;
	}
}

void LoadByteBGRA::io(Pointer row, Pointer data, UInt width)
{
    Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		b[u] = ptr[i++] << 8;
		g[u] = ptr[i++] << 8;
		r[u] = ptr[i++] << 8;
        a[u] = ptr[i++];
	}
}

void WriteByteBGRA::io(Pointer row, Pointer data, UInt width)
{
    Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		ptr[i++] = b[u] >> 8;
		ptr[i++] = g[u] >> 8;
		ptr[i++] = r[u] >> 8;
        ptr[i++] = (Byte)a[u];
	}
}

void LoadByteRGBA::io(Pointer row, Pointer data, UInt width)
{
    Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		r[u] = ptr[i++] << 8;
		g[u] = ptr[i++] << 8;
		b[u] = ptr[i++] << 8;
        a[u] = ptr[i++];
	}
}

void WriteByteRGBA::io(Pointer row, Pointer data, UInt width)
{
    Byte *ptr = (Byte*)row;
	
	Word **data0 = (Word**)data;
	Word *r = data0[0];
	Word *g = data0[1];
	Word *b = data0[2];
    Word *a = data0[3];
	
	UInt i = 0;
	for (UInt u = 0; u < width; u++)
	{
		ptr[i++] = r[u] >> 8;
		ptr[i++] = g[u] >> 8;
		ptr[i++] = b[u] >> 8;
        ptr[i++] = (Byte)a[u];
	}
}

void CvtWordColorAlphaToWordMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *r = src0[0];
	Word *g = src0[1];
	Word *b = src0[2];
	
	Word **dst0 = (Word**)dst;
	Word *val = dst0[0];
	
	for (UInt i = 0; i < width; i++)
	{
		val[i] = (Word)(table.RToY[r[i]] + table.GToY[g[i]] + table.BToY[b[i]]);
	}
}

void CvtWordMonoToWordColorAlpha::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *val = src0[0];
	
	Word **dst0 = (Word**)dst;
	Word *r = dst0[0];
	Word *g = dst0[1];
	Word *b = dst0[2];
    Word *a = dst0[3];
	
	for (UInt i = 0; i < width; i++)
	{
		r[i] = val[i];
		g[i] = val[i];
		b[i] = val[i];
        a[i] = 255;
	}
}

void CvtWordColorAlphaToFloatColor::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *r1 = src0[0];
	Word *g1 = src0[1];
	Word *b1 = src0[2];
	
	Float **dst0 = (Float**)dst;
	Float *r2 = dst0[0];
	Float *g2 = dst0[1];
	Float *b2 = dst0[2];
	
	for (UInt i = 0; i < width; i++)
	{
		r2[i] = WORD2FLOAT * r1[i];
		g2[i] = WORD2FLOAT * g1[i];
		b2[i] = WORD2FLOAT * b1[i];
	}
}

void CvtFloatColorToWordColorAlpha::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *r1 = src0[0];
	Float *g1 = src0[1];
	Float *b1 = src0[2];
	
	Word **dst0 = (Word**)dst;
	Word *r2 = dst0[0];
	Word *g2 = dst0[1];
	Word *b2 = dst0[2];
    Word *a2 = dst0[3];
	
	for (UInt i = 0; i < width; i++)
	{
		r2[i] = (Word)round(65535.0f * r1[i]);
		g2[i] = (Word)round(65535.0f * g1[i]);
		b2[i] = (Word)round(65535.0f * b1[i]);
        a2[i] = 255;
	}
}

void CvtWordColorAlphaToFloatMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *r = src0[0];
	Word *g = src0[1];
	Word *b = src0[2];
	
	Float **dst0 = (Float**)dst;
	Float *val = dst0[0];
	
	for (UInt i = 0; i < width; i++)
	{
		val[i] = WORD2FLOAT * (table.RToY[r[i]] + table.GToY[g[i]] + table.BToY[b[i]]);
	}
}

void CvtFloatMonoToWordColorAlpha::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *val = src0[0];
	
	Word **dst0 = (Word**)dst;
	Word *r = dst0[0];
	Word *g = dst0[1];
	Word *b = dst0[2];
    Word *a = dst0[3];
	
	for (UInt i = 0; i < width; i++)
	{
		Word theVal = (Word)round(65535.0f * val[i]);
		r[i] = theVal;
		g[i] = theVal;
		b[i] = theVal;
        a[i] = 255;
	}
}

void CvtWordMonoToFloatColor::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *val = src0[0];
	
	Float **dst0 = (Float**)dst;
	Float *r = dst0[0];
	Float *g = dst0[1];
	Float *b = dst0[2];
	
	for (UInt i = 0; i < width; i++)
	{
		Float theVal = WORD2FLOAT * val[i];
		r[i] = theVal;
		g[i] = theVal;
		b[i] = theVal;
	}
}

void CvtFloatColorToWordMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *r = src0[0];
	Float *g = src0[1];
	Float *b = src0[2];
	
	Word **dst0 = (Word**)dst;
	Word *val = dst0[0];
	
	for (UInt i = 0; i < width; i++)
	{
		val[i] = (Word)round(19589.0f * r[i] + 38443.0f * g[i] + 7502.0f * b[i]);
	}
}

void CvtWordMonoToFloatMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *val1 = src0[0];
	
	Float **dst0 = (Float**)dst;
	Float *val2 = dst0[0];
	
	for (UInt i = 0; i < width; i++)
	{
		val2[i] = WORD2FLOAT * val1[i];
	}
}

void CvtFloatMonoToWordMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *val1 = src0[0];
	
	Word **dst0 = (Word**)dst;
	Word *val2 = dst0[0];
	
	for (UInt i = 0; i < width; i++)
	{
		val2[i] = (Word)round(65535.0f * val1[i]);
	}
}

void CvtFloatColorToFloatMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *r = src0[0];
	Float *g = src0[1];
	Float *b = src0[2];
	
	Float **dst0 = (Float**)dst;
	Float *val = dst0[0];
	
	for (UInt i = 0; i < width; i++)
	{
		val[i] = 0.29891f * r[i] + 0.58661f * g[i] + 0.11448f * b[i];
	}
}

void CvtFloatMonoToFloatColor::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *val = src0[0];
	
	Float **dst0 = (Float**)dst;
	Float *r = dst0[0];
	Float *g = dst0[1];
	Float *b = dst0[2];
	
	for (UInt i = 0; i < width; i++)
	{
		r[i] = val[i];
		g[i] = val[i];
		b[i] = val[i];
	}
}

void FlipWordColorAlpha::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *r1 = src0[0];
	Word *g1 = src0[1];
	Word *b1 = src0[2];
    Word *a1 = src0[3];
	
	Word **dst0 = (Word**)dst;
	Word *r2 = dst0[0];
	Word *g2 = dst0[1];
	Word *b2 = dst0[2];
    Word *a2 = dst0[3];
	
	for (UInt i1 = 0, i2 = width-1; i1 < width; i1++, i2--)
	{
		r2[i2] = r1[i1];
		g2[i2] = g1[i1];
		b2[i2] = b1[i1];
        a2[i2] = a1[i1];
	}
}

void FlipWordMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Word **src0 = (Word**)src;
	Word *val1 = src0[0];
	
	Word **dst0 = (Word**)dst;
	Word *val2 = dst0[0];
	
	for (UInt i1 = 0, i2 = width-1; i1 < width; i1++, i2--)
	{
		val2[i2] = val1[i1];
	}
}

void FlipFloatColor::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *r1 = src0[0];
	Float *g1 = src0[1];
	Float *b1 = src0[2];
	
	Float **dst0 = (Float**)dst;
	Float *r2 = dst0[0];
	Float *g2 = dst0[1];
	Float *b2 = dst0[2];
	
	for (UInt i1 = 0, i2 = width-1; i1 < width; i1++, i2--)
	{
		r2[i2] = r1[i1];
		g2[i2] = g1[i1];
		b2[i2] = b1[i1];
	}
}

void FlipFloatMono::cvt(Pointer src, Pointer dst, UInt width)
{
	Float **src0 = (Float**)src;
	Float *val1 = src0[0];
	
	Float **dst0 = (Float**)dst;
	Float *val2 = dst0[0];
	
	for (UInt i1 = 0, i2 = width-1; i1 < width; i1++, i2--)
	{
		val2[i2] = val1[i1];
	}
}

void ResizeWordColorAlpha::rsz(Pointer* src, Pointer dst, UInt srcWidth, UInt dstWidth, UInt srcHeight, UInt dstHeight, Enum<ResizeMode> mode, Float v)
{
	if (mode == ResizeMode::Nearest)
	{
		Word** src0 = (Word**)src[0];
		Word *r1 = src0[0];
		Word *g1 = src0[1];
		Word *b1 = src0[2];
        Word *a1 = src0[3];
		
		Word **dst0 = (Word**)dst;
		Word *r2 = dst0[0];
		Word *g2 = dst0[1];
		Word *b2 = dst0[2];
        Word *a2 = dst0[3];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		for (UInt x = 0; x < dstWidth; x++)
		{
			u = x * scaleWidth;
			r2[x] = r1[(Int)u];
			g2[x] = g1[(Int)u];
			b2[x] = b1[(Int)u];
            a2[x] = a1[(Int)u];
		}
	}
	else if(mode == ResizeMode::Bilinear)
	{
		Word*** src0 = (Word***)src;
		
		Word **dst0 = (Word**)dst;
		Word *r2 = dst0[0];
		Word *g2 = dst0[1];
		Word *b2 = dst0[2];
        Word *a2 = dst0[3];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		Int floorv = floor(v);
		Int ceilv = floorv + 1;
		
		Word *row0R = src0[0][0];
		Word *row0G = src0[0][1];
		Word *row0B = src0[0][2];
        Word *row0A = src0[0][3];
		Word *row1R = src0[1][0];
		Word *row1G = src0[1][1];
		Word *row1B = src0[1][2];
        Word *row1A = src0[1][3];
		for (UInt x = 0; x < dstWidth; x++)
		{
			Float dstr = 0;
			Float dstg = 0;
			Float dstb = 0;
            Float dsta = 0;
			
			u = x * scaleWidth;
			Int flooru = floor(u);
			Int ceilu = flooru + 1;
			
			Float aa = (ceilu - u) * (ceilv - v);
			Float bb = (u - flooru) * (ceilv - v);
			Float cc = (ceilu - u) * (v - floorv);
			Float dd = (u - flooru) * (v - floorv);
			
			dstr += row0R[flooru] * aa;
			dstg += row0G[flooru] * aa;
			dstb += row0B[flooru] * aa;
            dsta += row0A[flooru] * aa;
			
			dstr += row0R[ceilu] * bb;
			dstg += row0G[ceilu] * bb;
			dstb += row0B[ceilu] * bb;
            dsta += row0A[ceilu] * bb;
			
			dstr += row1R[flooru] * cc;
			dstg += row1G[flooru] * cc;
			dstb += row1B[flooru] * cc;
            dsta += row1A[flooru] * cc;
			
			dstr += row1R[ceilu] * dd;
			dstg += row1G[ceilu] * dd;
			dstb += row1B[ceilu] * dd;
            dsta += row1A[ceilu] * dd;
			
			r2[x] = (Word)round(dstr);
			g2[x] = (Word)round(dstg);
			b2[x] = (Word)round(dstb);
            a2[x] = (Word)round(dsta);
		}
	}
}

void ResizeWordMono::rsz(Pointer* src, Pointer dst, UInt srcWidth, UInt dstWidth, UInt srcHeight, UInt dstHeight, Enum<ResizeMode> mode, Float v)
{
	if (mode == ResizeMode::Nearest)
	{
		Word** src0 = (Word**)src[0];
		Word *v1 = src0[0];
		
		Word **dst0 = (Word**)dst;
		Word *v2 = dst0[0];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		for (UInt x = 0; x < dstWidth; x++)
		{
			u = x * scaleWidth;
			v2[x] = v1[(Int)u];
		}
	}
	else if(mode == ResizeMode::Bilinear)
	{
		Word*** src0 = (Word***)src;
		
		Word **dst0 = (Word**)dst;
		Word *val2 = dst0[0];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		Int floorv = floor(v);
		Int ceilv = floorv + 1;
		for (UInt x = 0; x < dstWidth; x++)
		{
			Float dst = 0;
			
			u = x * scaleWidth;
			Int flooru = floor(u);
			Int ceilu = flooru + 1;
			Word *row0 = src0[0][0];
			Word *row1 = src0[1][0];
			Float aa = (ceilu - u) * (ceilv - v);
			Float bb = (u - flooru) * (ceilv - v);
			Float cc = (ceilu - u) * (v - floorv);
			Float dd = (u - flooru) * (v - floorv);
			
			dst += row0[flooru] * aa;
			dst += row0[ceilu] * bb;
			dst += row1[flooru] * cc;
			dst += row1[ceilu] * dd;
			
			val2[x] = (Word)round(dst);
		}
	}
}

void ResizeFloatColor::rsz(Pointer* src, Pointer dst, UInt srcWidth, UInt dstWidth, UInt srcHeight, UInt dstHeight, Enum<ResizeMode> mode, Float v)
{
	if (mode == ResizeMode::Nearest)
	{
		Float** src0 = (Float**)src[0];
		Float *r1 = src0[0];
		Float *g1 = src0[1];
		Float *b1 = src0[2];
		
		Float **dst0 = (Float**)dst;
		Float *r2 = dst0[0];
		Float *g2 = dst0[1];
		Float *b2 = dst0[2];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		for (UInt x = 0; x < dstWidth; x++)
		{
			u = x * scaleWidth;
			r2[x] = r1[(Int)u];
			g2[x] = g1[(Int)u];
			b2[x] = b1[(Int)u];
		}
	}
	else if(mode == ResizeMode::Bilinear)
	{
		Float*** src0 = (Float***)src;
		
		Float **dst0 = (Float**)dst;
		Float *r2 = dst0[0];
		Float *g2 = dst0[1];
		Float *b2 = dst0[2];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		Int floorv = floor(v);
		Int ceilv = floorv + 1;
		for (UInt x = 0; x < dstWidth; x++)
		{
			Float dstr = 0;
			Float dstg = 0;
			Float dstb = 0;
			
			u = x * scaleWidth;
			Int flooru = floor(u);
			Int ceilu = flooru + 1;
			Float *row0R = src0[0][0];
			Float *row0G = src0[0][1];
			Float *row0B = src0[0][2];
			Float *row1R = src0[1][0];
			Float *row1G = src0[1][1];
			Float *row1B = src0[1][2];
			Float aa = (ceilu - u) * (ceilv - v);
			Float bb = (u - flooru) * (ceilv - v);
			Float cc = (ceilu - u) * (v - floorv);
			Float dd = (u - flooru) * (v - floorv);
			
			dstr += row0R[flooru] * aa;
			dstg += row0G[flooru] * aa;
			dstb += row0B[flooru] * aa;
			
			dstr += row0R[ceilu] * bb;
			dstg += row0G[ceilu] * bb;
			dstb += row0B[ceilu] * bb;
			
			dstr += row1R[flooru] * cc;
			dstg += row1G[flooru] * cc;
			dstb += row1B[flooru] * cc;
			
			dstr += row1R[ceilu] * dd;
			dstg += row1G[ceilu] * dd;
			dstb += row1B[ceilu] * dd;
			
			r2[x] = dstr;
			g2[x] = dstg;
			b2[x] = dstb;
		}
	}
}

void ResizeFloatMono::rsz(Pointer* src, Pointer dst, UInt srcWidth, UInt dstWidth, UInt srcHeight, UInt dstHeight, Enum<ResizeMode> mode, Float v)
{
	if (mode == ResizeMode::Nearest)
	{
		Float** src0 = (Float**)src[0];
		Float *v1 = src0[0];
		
		Float **dst0 = (Float**)dst;
		Float *v2 = dst0[0];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		for (UInt x = 0; x < dstWidth; x++)
		{
			u = x * scaleWidth;
			v2[x] = v1[(Int)u];
		}
	}
	else if(mode == ResizeMode::Bilinear)
	{
		Float*** src0 = (Float***)src;
		
		Float **dst0 = (Float**)dst;
		Float *val2 = dst0[0];
		
		Float u = 0;
		Float scaleWidth = 0;
		if (dstWidth == 1)
		{
			scaleWidth = 0;
		}
		else
		{
			scaleWidth = (Float)(srcWidth - 1) / (dstWidth - 1);
		}
		Int floorv = floor(v);
		Int ceilv = floorv + 1;
		for (UInt x = 0; x < dstWidth; x++)
		{
			Float dst = 0;
			
			u = x * scaleWidth;
			Int flooru = floor(u);
			Int ceilu = flooru + 1;
			Float *row0 = src0[0][0];
			Float *row1 = src0[1][0];
			Float aa = (ceilu - u) * (ceilv - v);
			Float bb = (u - flooru) * (ceilv - v);
			Float cc = (ceilu - u) * (v - floorv);
			Float dd = (u - flooru) * (v - floorv);
			
			dst += row0[flooru] * aa;
			dst += row0[ceilu] * bb;
			dst += row1[flooru] * cc;
			dst += row1[ceilu] * dd;
			
			val2[x] = dst;
		}
	}
}

RowIOType image_internal::getRowIOType(Enum<PixelFormat> format)
{
	switch (format.value())
	{
		case PixelFormat::ByteBGR:
        case PixelFormat::WordBGR:
        case PixelFormat::ByteRGB:
        case PixelFormat::ByteUYVY:
        case PixelFormat::ByteYUV:
        case PixelFormat::ByteBGRA:
        case PixelFormat::ByteRGBA:
			return RowIOType::WordColorAlpha;
			
		case PixelFormat::ByteGray:
        case PixelFormat::ByteBool:
		case PixelFormat::WordGray:
			return RowIOType::WordMono;
			
		case PixelFormat::FloatBGR:
        case PixelFormat::FloatHSV:
			return RowIOType::FloatColor;
			
		case PixelFormat::FloatGray:
			return RowIOType::FloatMono;
			
		default:
			return RowIOType::WordColorAlpha;
	}
}

Pointer image_internal::createRowData(RowIOType type, UInt width)
{
	if (type == RowIOType::WordColorAlpha)
	{
		Word **data = new Word*[4];
		data[0] = new Word[width];	// r
		data[1] = new Word[width];	// g
		data[2] = new Word[width];	// b
        data[3] = new Word[width];	// a
		return (Pointer)data;
	}
	if (type == RowIOType::WordMono)
	{
		Word **data = new Word*[1];
		data[0] = new Word[width];
		return (Pointer)data;
	}
	if (type == RowIOType::FloatColor)
	{
		Float **data = new Float*[3];
		data[0] = new Float[width];	// r
		data[1] = new Float[width];	// g
		data[2] = new Float[width];	// b
		return (Pointer)data;
	}
	if (type == RowIOType::FloatMono)
	{
		Float **data = new Float*[1];
		data[0] = new Float[width];
		return (Pointer)data;
	}
	
	return NULL;
}

void image_internal::destroyRowData(Pointer data, RowIOType type)
{
	if (type == RowIOType::WordColorAlpha)
	{
		Word **data0 = (Word**)data;
		delete[] data0[0];
		delete[] data0[1];
		delete[] data0[2];
        delete[] data0[3];
		delete[] data0;
	}
	if (type == RowIOType::WordMono)
	{
		Word **data0 = (Word**)data;
		delete[] data0[0];
		delete[] data0;
	}
	if (type == RowIOType::FloatColor)
	{
		Float **data0 = (Float**)data;
		delete[] data0[0];
		delete[] data0[1];
		delete[] data0[2];
		delete[] data0;
	}
	if (type == RowIOType::FloatMono)
	{
		Float **data0 = (Float**)data;
		delete[] data0[0];
		delete[] data0;
	}
}

IRowIO *image_internal::createRowLoader(Enum<PixelFormat> format)
{
	switch (format.value())
	{
		case PixelFormat::ByteBGR:
			return new LoadByteBGR();
			
		case PixelFormat::ByteGray:
			return new LoadByteGray();
			
		case PixelFormat::WordBGR:
			return new LoadWordBGR();
			
		case PixelFormat::WordGray:
			return new LoadWordGray();
			
		case PixelFormat::FloatBGR:
			return new LoadFloatBGR();
			
		case PixelFormat::FloatGray:
			return new LoadFloatGray();
			
		case PixelFormat::ByteRGB:
			return new LoadByteRGB();

		case PixelFormat::ByteUYVY:
			return new LoadByteUYVY();
			
		case PixelFormat::ByteYUV:
			return new LoadByteYUV();

		case PixelFormat::FloatHSV:
			return new LoadFloatHSV();
			
		case PixelFormat::ByteBool:
			return new LoadByteBool();
            
        case PixelFormat::ByteBGRA:
            return new LoadByteBGRA();
            
        case PixelFormat::ByteRGBA:
            return new LoadByteRGBA();
			
		default:
			return NULL;
	}
}

IRowIO *image_internal::createRowWriter(Enum<PixelFormat> format)
{
	switch (format.value())
	{
		case PixelFormat::ByteBGR:
			return new WriteByteBGR();
			
		case PixelFormat::ByteGray:
			return new WriteByteGray();
			
		case PixelFormat::WordBGR:
			return new WriteWordBGR();
			
		case PixelFormat::WordGray:
			return new WriteWordGray();
			
		case PixelFormat::FloatBGR:
			return new WriteFloatBGR();
			
		case PixelFormat::FloatGray:
			return new WriteFloatGray();
			
		case PixelFormat::ByteRGB:
			return new WriteByteRGB();

		case PixelFormat::ByteUYVY:
			return new WriteByteUYVY();
			
		case PixelFormat::ByteYUV:
			return new WriteByteYUV();

		case PixelFormat::FloatHSV:
			return new WriteFloatHSV();
			
		case PixelFormat::ByteBool:
			return new WriteByteBool();
            
        case PixelFormat::ByteBGRA:
            return new WriteByteBGRA();
            
        case PixelFormat::ByteRGBA:
            return new WriteByteRGBA();
			
		default:
			return NULL;
	}
}

IRowCvt *image_internal::createRowCvt(RowIOType srcType, RowIOType dstType)
{
	switch (srcType)
	{
		case RowIOType::WordColorAlpha:
			switch (dstType)
		{
			case RowIOType::WordMono:
				return new CvtWordColorAlphaToWordMono();
				
			case RowIOType::FloatColor:
				return new CvtWordColorAlphaToFloatColor();
				
			case RowIOType::FloatMono:
				return new CvtWordColorAlphaToFloatMono();
			default:
				break;
		}
			break;
			
		case RowIOType::WordMono:
			switch (dstType)
		{
			case RowIOType::WordColorAlpha:
				return new CvtWordMonoToWordColorAlpha();
				
			case RowIOType::FloatColor:
				return new CvtWordMonoToFloatColor();
				
			case RowIOType::FloatMono:
				return new CvtWordMonoToFloatMono();
				
			default:
				break;
		}
			break;
			
		case RowIOType::FloatColor:
			switch (dstType)
		{
			case RowIOType::WordColorAlpha:
				return new CvtFloatColorToWordColorAlpha();
				
			case RowIOType::WordMono:
				return new CvtFloatColorToWordMono();
				
			case RowIOType::FloatMono:
				return new CvtFloatColorToFloatMono();
			default:
				break;
		}
			break;
			
		case RowIOType::FloatMono:
			switch (dstType)
		{
			case RowIOType::WordColorAlpha:
				return new CvtFloatMonoToWordColorAlpha();
				
			case RowIOType::WordMono:
				return new CvtFloatMonoToWordMono();
				
			case RowIOType::FloatColor:
				return new CvtFloatMonoToFloatColor();
			default:
				break;
		}
			break;
	}
	
	return NULL;
}

IRowCvt *image_internal::createRowFlip(RowIOType type)
{
	switch (type)
	{
		case RowIOType::WordColorAlpha:
			return new FlipWordColorAlpha();
			
		case RowIOType::WordMono:
			return new FlipWordMono();
			
		case RowIOType::FloatColor:
			return new FlipFloatColor();
			
		case RowIOType::FloatMono:
			return new FlipFloatMono();
	}
	
	return NULL;
}

IRowRsz* image_internal::createRowRsz(RowIOType type)
{
	switch (type)
	{
		case RowIOType::WordColorAlpha:
			return new ResizeWordColorAlpha();
			
		case RowIOType::WordMono:
			return new ResizeWordMono();
			
		case RowIOType::FloatColor:
			return new ResizeFloatColor();
			
		case RowIOType::FloatMono:
			return new ResizeFloatMono();
	}
	
	return NULL;
}