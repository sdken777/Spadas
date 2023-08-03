
#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)

#include "string_spadas.h"
#include "3party/ConvertUTF.h"

using namespace spadas;
using namespace spadas::math;

UInt string_internal::utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	Bool srcCopied = FALSE;
    if (src[srcLength] != 0)
    {
    	Char *src2 = new Char[srcLength+1];
    	for (UInt i = 0; i < srcLength; i++) src2[i] = src[i];
    	src2[srcLength] = 0;
    	src = src2;
    	srcCopied = TRUE;
    }
    
    const UTF8* a = (const UTF8*)&src[0];
    const UTF8* b = (const UTF8*)&src[srcLength + 1];
    UTF32* c = (UTF32*)&dst[0];
    UTF32* d = (UTF32*)&dst[dstSize];
    
    ConvertUTF8toUTF32(&a, b, &c, d, strictConversion);
    dst[dstSize - 1] = 0;
    
    if (srcCopied) delete[] src;

    return (Int)((ULong)c - (ULong)dst) / 4 - 1;
}

UInt string_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	Bool srcCopied = FALSE;
    if (src[srcLength] != 0)
    {
    	WChar *src2 = new WChar[srcLength + 1];
    	for (UInt i = 0; i < srcLength; i++) src2[i] = src[i];
    	src2[srcLength] = 0;
    	src = src2;
    	srcCopied = true;
    }
    
    const UTF32* a = (const UTF32*)&src[0];
    const UTF32* b = (const UTF32*)&src[srcLength+1];
    UTF8* c = (UTF8*)&dst[0];
    UTF8* d = (UTF8*)&dst[dstSize];
    
    ConvertUTF32toUTF8(&a, b, &c, d, strictConversion);
    dst[dstSize - 1] = 0;
    
    if (srcCopied) delete[] src;

    return (Int)((ULong)c - (ULong)dst) - 1;
}

UInt string_internal::printToString(Char dst[], UInt dstSize, const Char format[], ...)
{
    int result = 0;
	va_list list;
	va_start(list, format);
	{
		result = vsnprintf(dst, dstSize, format, list);
	}
	va_end(list);
	if (result <= 0)
	{
		dst[dstSize - 1] = 0;
		return dstSize - 1;
	}
	else return math::min((UInt)result, dstSize - 1);
}

#endif

