
#if defined(SPADAS_ENV_WINDOWS)

#include "string_spadas.h"
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#undef min
#undef max

using namespace spadas;

UInt string_internal::utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	int res = MultiByteToWideChar(CP_UTF8, 0, src, srcLength, dst, dstSize);
	if (res == 0)
	{
		dst[dstSize - 1] = 0;
		return dstSize - 1;
	}
	else
	{
		UInt length = math::min((UInt)res, dstSize - 1);
		dst[length] = 0;
		return length;
	}
}

UInt string_internal::wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize)
{
	int res = WideCharToMultiByte(CP_UTF8, 0, src, srcLength, dst, dstSize, 0, 0);
	if (res == 0)
	{
		dst[dstSize - 1] = 0;
		return dstSize - 1;
	}
	else
	{
		UInt length = math::min((UInt)res, dstSize - 1);
		dst[length] = 0;
		return length;
	}
}

UInt string_internal::printToString(Char dst[], UInt dstSize, const Char format[], ...)
{
	int result = 0;
	va_list list;
	va_start(list, format);
	{
		result = _vsnprintf_s(dst, dstSize, dstSize - 1, format, list);
	}
	va_end(list);
	if (result <= 0)
	{
		dst[dstSize - 1] = 0;
		return dstSize - 1;
	}
	else return result;
}

UInt string_internal::ansiToWChar(const Byte src[], UInt srcLength, WChar dst[], UInt dstSize)
{
	return MultiByteToWideChar(CP_ACP, 0, (const Char*)src, srcLength, dst, dstSize);
}

UInt string_internal::wCharToAnsi(const WChar src[], UInt srcLength, Byte dst[], UInt dstSize)
{
	return WideCharToMultiByte(CP_ACP, 0, src, srcLength, (Char*)dst, dstSize, 0, 0);
}

#endif
