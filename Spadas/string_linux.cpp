
#include "spadas.h"

#include "string_bz.h"
#include "string_encoding.h"

#if defined(SPADAS_ENV_LINUX)

#include <stdio.h>
#include <string.h>
#include <wchar.h>

using namespace spadas;
using namespace spadas::math;
using namespace string_internal;

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
	return cvtUTF8toUTF32(src, srcLength, dst, dstSize);
}

UInt spadas_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	return cvtUTF32toUTF8(src, srcLength, dst, dstSize);
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

