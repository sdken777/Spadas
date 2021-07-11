
#include "spadas.h"

#include "string_bz.h"
#include "string_encoding.h"

#if defined(SPADAS_ENV_LINUX)

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

Int spadas_internal::compareString(const WChar str1[], const WChar str2[])
{
	return wcscmp(str1, str2);
}

UInt spadas_internal::charToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	return cvtUTF8toUTF32(src, srcLength, dst, dstSize);
}

UInt spadas_internal::wCharToChar(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
    return cvtUTF32toUTF8(src, srcLength, dst, dstSize);
}

UInt spadas_internal::utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	return cvtUTF8toUTF32(src, srcLength, dst, dstSize);
}

UInt spadas_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	return cvtUTF32toUTF8(src, srcLength, dst, dstSize);
}

UInt spadas_internal::printToString(WChar dst[], UInt dstSize, const WChar format[], ...)
{
    UInt length = 0;
	va_list list;
	va_start(list, format);
	{
		length = vswprintf(dst, dstSize, format, list);
	}
	va_end(list);
	dst[dstSize - 1] = L'\0';
    return length;
}

Bool spadas_internal::scanFromString(const WChar src[], UInt srcSize, const WChar format[], Pointer val)
{
	return swscanf(src, format, val) == 1;
}

#endif

