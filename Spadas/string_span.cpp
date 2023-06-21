
#include "string_spadas.h"

using namespace string_internal;

StringSpan::StringSpan() : idx(0), len(0)
{}

StringSpan::StringSpan(String& sourceString, UInt index, UInt length) : source(sourceString), idx(index), len(length)
{}

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

const Byte *StringSpan::bytes()
{
	return len == 0 ? NULL : (source.bytes() + idx);
}

UInt StringSpan::length()
{
	return len;
}

Bool StringSpan::isEmpty()
{
	return len == 0;
}

Word StringSpan::getHash()
{
	if (len) return StringCommon::getHashCode(source.bytes() + idx, len);
	else return 0;
}

String StringSpan::clone()
{
	if (len) return StringCommon::clone(source.bytes() + idx, len);
	else return String();
}

Array<Char> StringSpan::chars()
{
	return StringCommon::chars(bytes(), len);
}

Array<WChar> StringSpan::wchars()
{
	return StringCommon::wchars(bytes(), len);
}

StringAppender StringSpan::operator +(String string)
{
	return StringCommon::operatorPlus(bytes(), len, string);
}

Optional<Int> StringSpan::toInt()
{
	if (len) return StringCommon::toInt(source.bytes() + idx, len);
	else return Optional<Int>();
}

Optional<Long> StringSpan::toLong()
{
	if (len) return StringCommon::toLong(source.bytes() + idx, len);
	else return Optional<Long>();
}

Optional<Float> StringSpan::toFloat()
{
	if (len) return StringCommon::toFloat(source.bytes() + idx, len);
	else return Optional<Float>();
}

Optional<Double> StringSpan::toDouble()
{
	if (len) return StringCommon::toDouble(source.bytes() + idx, len);
	else return Optional<Double>();
}

Bool StringSpan::toNumber(Int& number)
{
	if (len) return StringCommon::toNumber(source.bytes() + idx, len, number);
	else return FALSE;
}

Bool StringSpan::toNumber(Long& number)
{
	if (len) return StringCommon::toNumber(source.bytes() + idx, len, number);
	else return FALSE;
}

Bool StringSpan::toNumber(Float& number)
{
	if (len) return StringCommon::toNumber(source.bytes() + idx, len, number);
	else return FALSE;
}

Bool StringSpan::toNumber(Double& number)
{
	if (len) return StringCommon::toNumber(source.bytes() + idx, len, number);
	else return FALSE;
}

Binary StringSpan::toBinary()
{
	if (len) return StringCommon::toBinary(source.bytes() + idx, len);
	else return Binary();
}

String StringSpan::toUpper()
{
	if (len) return StringCommon::toUpper(source.bytes() + idx, len);
	else return String();
}

String StringSpan::toLower()
{
	if (len) return StringCommon::toLower(source.bytes() + idx, len);
	else return String();
}

Bool StringSpan::startsWith(String target)
{
	if (len) return StringCommon::startsWith(source.bytes() + idx, len, target);
	else return FALSE;
}

Bool StringSpan::endsWith(String target)
{
	if (len) return StringCommon::endsWith(source.bytes() + idx, len, target);
	else return FALSE;
}

Array<UInt> StringSpan::search(String target)
{
	return StringCommon::search(bytes(), len, target);
}

Array<StringSpan> StringSpan::split(String target)
{
	if (len) return StringCommon::split(source, idx, len, target);
	else return Array<StringSpan>();
}

String StringSpan::replace(String oldString, String newString)
{
	return StringCommon::replace(bytes(), len, oldString, newString);
}

StringSpan StringSpan::sub(UInt index, UInt length, Bool trimStart, Bool trimEnd)
{
	SPADAS_ERROR_RETURNVAL(len == 0, StringSpan());
	return StringCommon::sub(source, idx, len, index, length, trimStart, trimEnd);
}