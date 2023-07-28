
#if defined(SPADAS_DEBUG)
#undef SPADAS_DEBUG
#endif

#include "spadas.h"

#include "string_bz.h"
#include "3party/ConvertUTF.h"

#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <stdio.h>
#include <string.h>
#include <wchar.h>

using namespace spadas;
using namespace spadas::math;

UInt spadas_internal::lengthChar(const Char str[])
{
	return (UInt)strlen(str);
}

UInt spadas_internal::lengthWChar(const WChar str[])
{
	return (UInt)wcslen(str);
}

Int spadas_internal::compareString(const Char str1[], const Char str2[])
{
	return strcmp(str1, str2);
}

UInt spadas_internal::charToUTF8(const Char src[], UInt srcLength, Char dst[], UInt dstSize)
{
	UInt length = math::min(srcLength, dstSize - 1);
	utility::memoryCopy((const Pointer)src, dst, length);
	dst[length] = 0;
	return length;
}

UInt spadas_internal::utf8ToChar(const Char src[], UInt srcLength, Char dst[], UInt dstSize)
{
	UInt length = math::min(srcLength, dstSize - 1);
	utility::memoryCopy((const Pointer)src, dst, length);
	dst[length] = 0;
	return length;
}

UInt spadas_internal::utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
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
    const UTF8* b = (const UTF8*)&src[srcLength+1];
    UTF32* c = (UTF32*)&dst[0];
    UTF32* d = (UTF32*)&dst[dstSize];
    
    ConvertUTF8toUTF32(&a, b, &c, d, strictConversion);
    dst[dstSize-1] = 0;
    
    if (srcCopied) delete[] src;

    return (Int)((ULong)c - (ULong)dst) / 4 - 1;
}

UInt spadas_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	Bool srcCopied = FALSE;
    if (src[srcLength] != 0)
    {
    	WChar *src2 = new WChar[srcLength+1];
    	for (UInt i = 0; i < srcLength; i++) src2[i] = src[i];
    	src2[srcLength] = 0;
    	src = src2;
    	srcCopied = TRUE;
    }
    
    const UTF32* a = (const UTF32*)&src[0];
    const UTF32* b= (const UTF32*)&src[srcLength+1];
    UTF8* c = (UTF8*)&dst[0];
    UTF8* d = (UTF8*)&dst[dstSize];
    
    ConvertUTF32toUTF8(&a, b, &c, d, strictConversion);
    dst[dstSize-1] = 0;
    
    if (srcCopied) delete[] src;

    return (Int)((ULong)c - (ULong)dst) - 1;
}

UInt spadas_internal::printToString(Char dst[], UInt dstSize, const Char format[], ...)
{
    UInt length = 0;
	va_list list;
	va_start(list, format);
	{
		length = vsnprintf(dst, dstSize, format, list);
	}
	va_end(list);
	dst[dstSize - 1] = L'\0';
    return length;
}

Bool spadas_internal::scanFromString(const Char src[], UInt srcSize, const Char format[], Pointer val)
{
	return sscanf(src, format, val) == 1;
}

#endif

