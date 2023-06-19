
#include "string_spadas.h"

using namespace string_internal;

StringSpan::StringSpan() : idx(0), len(0)
{}

StringSpan::StringSpan(String& sourceString, UInt index, UInt length) : source(sourceString), idx(0), len(0)
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
	return StringCommon::chars(bytes(), length());
}

Array<WChar> StringSpan::wchars()
{
	return StringCommon::wchars(bytes(), length());
}

StringAppender StringSpan::operator +(String string)
{
	return StringCommon::operatorPlus(bytes(), length(), string);
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
	return StringCommon::search(bytes(), length(), target);
}

struct StringSpanStruct
{
public:
	String source;
	UInt idx;
	UInt len;
};

StringSpan genStringSpanFromStringSpan(Pointer obj, UInt index, UInt length)
{
	StringSpanStruct *thisSpan = (StringSpanStruct*)obj;
	SPADAS_ERROR_RETURNVAL(index >= thisSpan->len, StringSpan());

	ULong mergedIndex = (ULong)thisSpan->idx + index;
	SPADAS_ERROR_RETURNVAL(mergedIndex > UINF, StringSpan());

	return StringSpan(thisSpan->source, (UInt)mergedIndex, math::min(length, thisSpan->len - index));
}

Array<StringSpan> StringSpan::split(String target)
{
	return StringCommon::split(bytes(), length(), target, this, genStringSpanFromStringSpan);
}

String StringSpan::replace(String oldString, String newString)
{
	return StringCommon::replace(bytes(), length(), oldString, newString);
}

StringSpan StringSpan::subString(UInt index, UInt length, Bool trimStart, Bool trimEnd)
{
	return StringCommon::subString(index, length, trimStart, trimEnd, this, genStringSpanFromStringSpan);
}