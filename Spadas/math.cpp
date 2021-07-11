
#include "spadas.h"

#include <stdlib.h>
#include <math.h>

namespace math_internal
{
	using namespace spadas;

	Timer timerForRandom;

	Float getZeroF()
	{
		return 0.0f;
	}
	Double getZeroD()
	{
		return 0.0;
	}
	
	Float finfBase = 1.0f / getZeroF();
    Float nfinfBase = -1.0f / getZeroF();
	Double dinfBase = 1.0 / getZeroD();
    Double ndinfBase = -1.0 / getZeroD();
}

using namespace spadas;
using namespace spadas::math;
using namespace math_internal;

Float spadas::math::finf()
{
	return finfBase;
}

Float spadas::math::nfinf()
{
	return nfinfBase;
}

Double spadas::math::dinf()
{
	return dinfBase;
}

Double spadas::math::ndinf()
{
	return ndinfBase;
}

Float spadas::math::power(Float a, Float x)
{
	if (a > 0.0f) return (Float)pow(a, x);
	SPADAS_ERROR_RETURNVAL(a < 0.0f, FINF);
	return 0.0f;
}

Double spadas::math::power(Double a, Double x)
{
	if (a > 0.0) return pow(a, x);
	SPADAS_ERROR_RETURNVAL(a < 0.0, DINF);
	return 0.0;
}

Float spadas::math::exp(Float x)
{
	return (Float)::exp(x);
}

Double spadas::math::exp(Double x)
{
	return ::exp(x);
}

Float spadas::math::ln(Float x)
{
	SPADAS_ERROR_RETURNVAL(x <= 0.0f, FINF);
	return (Float)::log(x);
}

Double spadas::math::ln(Double x)
{
	SPADAS_ERROR_RETURNVAL(x <= 0.0, DINF);
	return ::log(x);
}

Float spadas::math::sin(Float x)
{
	return sinf(x * DEG2RAD);
}

Double spadas::math::sin(Double x)
{
	return ::sin(x * DEG2RAD);
}

Float spadas::math::cos(Float x)
{
	return cosf(x * DEG2RAD);
}

Double spadas::math::cos(Double x)
{
	return ::cos(x * DEG2RAD);
}

Float spadas::math::tan(Float x)
{
	return tanf(x * DEG2RAD);
}

Double spadas::math::tan(Double x)
{
	return ::tan(x * DEG2RAD);
}

Float spadas::math::asin(Float x)
{
	SPADAS_ERROR_RETURNVAL(x < -1.0f || x > 1.0f, FINF);
	return (Float)::asin(x) * RAD2DEG;
}

Double spadas::math::asin(Double x)
{
	SPADAS_ERROR_RETURNVAL(x < -1.0 || x > 1.0, DINF);
	return ::asin(x) * (Double)RAD2DEG;
}

Float spadas::math::acos(Float x)
{
	SPADAS_ERROR_RETURNVAL(x < -1.0f || x > 1.0f, FINF);
	return (Float)::acos(x) * RAD2DEG;
}

Double spadas::math::acos(Double x)
{
	SPADAS_ERROR_RETURNVAL(x < -1.0 || x > 1.0, DINF);
	return ::acos(x) * (Double)RAD2DEG;
}

Float spadas::math::atan(Float x, Float y)
{
	return atan2f(x, y) * RAD2DEG;
}

Double spadas::math::atan(Double x, Double y)
{
	return atan2(x, y) * RAD2DEG;
}

UInt spadas::math::depth(UInt number)
{
	UInt depth = 0;
	if (number & 0xffffff00)
	{
		if (number & 0xffff0000)
		{
			if (number & 0xff000000)
			{
				if (number & 0xf0000000)
				{
					if (number & 0xc0000000)
					{
						if (number & 0x80000000) depth = 31;
						else depth = 30;
					}
					else
					{
						if (number & 0xe0000000) depth = 29;
						else depth = 28;
					}
				}
				else
				{
					if (number & 0xfc000000)
					{
						if (number & 0xf8000000) depth = 27;
						else depth = 26;
					}
					else
					{
						if (number & 0xfe000000) depth = 25;
						else depth = 24;
					}
				}
			}
			else
			{
				if (number & 0xfff00000)
				{
					if (number & 0xffc00000)
					{
						if (number & 0xff800000) depth = 23;
						else depth = 22;
					}
					else
					{
						if (number & 0xffe00000) depth = 21;
						else depth = 20;
					}
				}
				else
				{
					if (number & 0xfffc0000)
					{
						if (number & 0xfff80000) depth = 19;
						else depth = 18;
					}
					else
					{
						if (number & 0xfffe0000) depth = 17;
						else depth = 16;
					}
				}
			}
		}
		else
		{
			if (number & 0xfffff000)
			{
				if (number & 0xffffc000)
				{
					if (number & 0xffff8000) depth = 15;
					else depth = 14;
				}
				else
				{
					if (number & 0xffffe000) depth = 13;
					else depth = 12;
				}
			}
			else
			{
				if (number & 0xfffffc00)
				{
					if (number & 0xfffff800) depth = 11;
					else depth = 10;
				}
				else
				{
					if (number & 0xfffffe00) depth = 9;
					else depth = 8;
				}
			}
		}
	}
	else
	{
		if (number & 0xfffffff0)
		{
			if (number & 0xffffffc0)
			{
				if (number & 0xffffff80) depth = 7;
				else depth = 6;
			}
			else
			{
				if (number & 0xffffffe0) depth = 5;
				else depth = 4;
			}
		}
		else
		{
			if (number & 0xfffffffc)
			{
				if (number & 0xfffffff8) depth = 3;
				else depth = 2;
			}
			else
			{
				if (number & 0xfffffffe) depth = 1;
				else depth = 0;
			}
		}
	}
	return depth;
}

Array<Float> spadas::math::random(UInt size)
{
	Double time = timerForRandom.check();
	UInt *timePtr = (UInt*)(Pointer)&time;

	srand(timePtr[0] + timePtr[1]);

	Array<Float> out(size);
	Float k = 1.0f / RAND_MAX;
	for (UInt i = 0; i < size; i++)
	{
		out[i] = (Float)rand() * k;
	}
	return out;
}

Array<Double> spadas::math::randomD(UInt size)
{
	Double time = timerForRandom.check();
	UInt *timePtr = (UInt*)(Pointer)&time;

	srand(timePtr[0] + timePtr[1]);

	Array<Double> out(size);
	Double k = 1.0f / RAND_MAX;
	for (UInt i = 0; i < size; i++)
	{
		out[i] = (Double)rand() * k;
	}
	return out;
}