
#include "spadas.h"

#include "string_bz.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <string.h>
#include <stdio.h>
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

Int spadas_internal::compareString(const Char str1[], const Char str2[])
{
	return strcmp(str1, str2);
}

UInt spadas_internal::utf8ToChar(const Char src[], UInt srcLength, Char dst[], UInt dstSize)
{
	Array<WChar> buffer(srcLength + 1);
	UInt wcharLength = math::min(srcLength, (UInt)MultiByteToWideChar(CP_UTF8, 0, src, srcLength, buffer.data(), buffer.size()));
	buffer[wcharLength] = 0;
	return WideCharToMultiByte(CP_ACP, 0, buffer.data(), wcharLength, dst, dstSize, "?", 0);
}

UInt spadas_internal::charToUTF8(const Char src[], UInt srcLength, Char dst[], UInt dstSize)
{
	Array<WChar> buffer(srcLength + 1);
	UInt wcharLength = math::min(srcLength, (UInt)MultiByteToWideChar(CP_ACP, 0, src, srcLength, buffer.data(), buffer.size()));
	buffer[wcharLength] = 0;
	return WideCharToMultiByte(CP_UTF8, 0, buffer.data(), wcharLength, dst, dstSize, 0, 0);
}

UInt spadas_internal::utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	return MultiByteToWideChar(CP_UTF8, 0, src, srcLength, dst, dstSize);
}

UInt spadas_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	return WideCharToMultiByte(CP_UTF8, 0, src, srcLength, dst, dstSize, 0, 0);
}

UInt spadas_internal::printToString(Char dst[], UInt dstSize, const Char format[], ...)
{
	UInt length = 0;
	va_list list;
	va_start(list, format);
	{
		
		length = _vsnprintf_s(dst, dstSize, dstSize - 1, format, list);
	}
	va_end(list);
	dst[dstSize - 1] = 0;
	return length;
}

Bool spadas_internal::scanFromString(const Char src[], UInt srcSize, const Char format[], Pointer val)
{
	return _snscanf_s(src, srcSize, format, val) == 1;
}

#endif
