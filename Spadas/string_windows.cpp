
#include "spadas.h"

#include "string_bz.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <string.h>
#include <wchar.h>
#include <windows.h>
#undef min
#undef max

using namespace spadas;

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
	return MultiByteToWideChar(CP_ACP, 0, src, srcLength, dst, dstSize);
}

UInt spadas_internal::wCharToChar(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	return WideCharToMultiByte(CP_ACP, 0, src, srcLength, dst, dstSize, "?", 0);
}

UInt spadas_internal::utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	return MultiByteToWideChar(CP_UTF8, 0, src, srcLength, dst, dstSize);
}

UInt spadas_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	return WideCharToMultiByte(CP_UTF8, 0, src, srcLength, dst, dstSize, 0, 0);
}

UInt spadas_internal::printToString(WChar dst[], UInt dstSize, const WChar format[], ...)
{
	UInt length = 0;
	va_list list;
	va_start(list, format);
	{
		length = _vsnwprintf_s(dst, dstSize, dstSize - 1, format, list);
	}
	va_end(list);
	dst[dstSize - 1] = 0;
	return length;
}

Bool spadas_internal::scanFromString(const WChar src[], UInt srcSize, const WChar format[], Pointer val)
{
	return _snwscanf_s(src, srcSize, format, val) == 1;
}

#endif
