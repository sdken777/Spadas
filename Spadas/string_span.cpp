
#include "string_spadas.h"

using namespace string_internal;

StringSpan::StringSpan() : idx(0), len(0), toStringCount(0)
{}

StringSpan::StringSpan(String& sourceString, UInt index, UInt length) : source(sourceString), idx(0), len(0), toStringCount(0)
{
	UInt srcLength = source.length();
	SPADAS_ERROR_RETURN(index >= srcLength);

	len = math::min(length, srcLength - index);
	if (len > 0) idx = index;
}

Bool StringSpan::operator ==(StringSpan span)
{
	if (len == 0)
	{
		return span.len == 0;
	}
	else
	{
		if (span.len == 0) return FALSE;
		else return compareString(source.bytes() + idx, len, span.source.bytes() + span.idx, span.len) == 0;
	}
}

Bool StringSpan::operator !=(StringSpan span)
{
	if (len == 0)
	{
		return span.len != 0;
	}
	else
	{
		if (span.len == 0) return TRUE;
		else return compareString(source.bytes() + idx, len, span.source.bytes() + span.idx, span.len) != 0;
	}
}

Bool StringSpan::operator ==(String string)
{
	UInt targetLength = string.length();
	if (len == 0)
	{
		return targetLength == 0;
	}
	else
	{
		if (targetLength == 0) return FALSE;
		else return compareString(source.bytes() + idx, len, string.bytes(), targetLength) == 0;
	}
}

Bool StringSpan::operator !=(String string)
{
	UInt targetLength = string.length();
	if (len == 0)
	{
		return targetLength != 0;
	}
	else
	{
		if (targetLength == 0) return TRUE;
		else return compareString(source.bytes() + idx, len, string.bytes(), targetLength) != 0;
	}
}

Bool StringSpan::operator >(StringSpan span)
{
	if (len == 0)
	{
		return FALSE;
	}
	else
	{
		if (span.len == 0) return TRUE;
		else return compareString(source.bytes() + idx, len, span.source.bytes() + span.idx, span.len) > 0;
	}
}

Bool StringSpan::operator <(StringSpan span)
{
	if (len == 0)
	{
		return span.len != 0;
	}
	else
	{
		if (span.len == 0) return FALSE;
		else return compareString(source.bytes() + idx, len, span.source.bytes() + span.idx, span.len) < 0;
	}
}

Word StringSpan::getHash()
{
	return getHashCode();
}

const Byte *StringSpan::bytes()
{
	return len == 0 ? NULL : (source.bytes() + idx);
}

UInt StringSpan::length()
{
	return len;
}

String StringSpan::toString()
{
	if (++toStringCount == 16) SPADAS_WARNING_MSG("Method called repeatedly");
	return clone();
}

StringSpan StringSpan::genStringSpan(UInt index, UInt length)
{
	SPADAS_ERROR_RETURNVAL(index >= len, StringSpan());

	ULong mergedIndex = (ULong)idx + index;
	SPADAS_ERROR_RETURNVAL(mergedIndex > UINF, StringSpan());

	return StringSpan(source, (UInt)mergedIndex, math::min(length, len - index));
}