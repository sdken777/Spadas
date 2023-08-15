
#include "string_spadas.h"

using namespace string_internal;

StringSpan::StringSpan() : source(0), idx(0), len(0)
{}

StringSpan::StringSpan(const Byte *ptr, UInt length) : source(0), idx(0), len(0)
{
	if (length == 0) return;
    SPADAS_ERROR_RETURN(ptr == NULL);

	Long k = length;
	const Byte *endPtr = ptr;
	while (--k >= 0 && *endPtr) endPtr++;

    source = (ULong)ptr;
    len = (UInt)(endPtr - ptr);
}

StringSpan::StringSpan(String& sourceString, UInt offset, UInt length) : source(0), idx(0), len(0)
{
	if (length == 0) return;

    StringVars *vars = sourceString.getVars();
    SPADAS_ERROR_RETURN(!vars);
    SPADAS_ERROR_RETURN(offset >= vars->length);

    vars->retain();
    source = (ULong)vars;
    idx = (UInt)((ULong)vars->data + offset - source);
    len = math::min(length, vars->length - offset);
}

StringSpan::StringSpan(StringSpan& sourceSpan, UInt offset, UInt length) : source(0), idx(0), len(0)
{
    if (length == 0) return;

    SPADAS_ERROR_RETURN(sourceSpan.source == 0);
    SPADAS_ERROR_RETURN(offset >= sourceSpan.len);
	
    if (sourceSpan.idx)
    {
        ((Vars*)sourceSpan.source)->retain();
        source = sourceSpan.source;
        idx = sourceSpan.idx + offset;
    }
    else
    {
        source = sourceSpan.source + offset;
    }
    len = math::min(length, sourceSpan.len - offset);
}

StringSpan::StringSpan(const StringSpan& span) : source(span.source), idx(span.idx), len(span.len)
{
    if (idx) ((Vars*)source)->retain();
}

StringSpan::~StringSpan()
{
    if (idx) ((Vars*)source)->release();
}

StringSpan& StringSpan::operator =(const StringSpan& span)
{
    if (idx) ((Vars*)source)->release();
    source = span.source;
    idx = span.idx;
    len = span.len;
    if (idx) ((Vars*)source)->retain();
    return *this;
}

Bool StringSpan::isRefCounting()
{
    return idx != 0;
}

Bool StringSpan::operator ==(StringSpan span)
{
	if (len != span.len) return FALSE;
	else if (len == 0) return TRUE;
	else return isSameString(bytes(), span.bytes(), len);
}

Bool StringSpan::operator !=(StringSpan span)
{
	if (len != span.len) return TRUE;
	else if (len == 0) return FALSE;
	else return !isSameString(bytes(), span.bytes(), len);
}

Bool StringSpan::operator ==(String string)
{
	UInt targetLength = string.length();
	if (len != targetLength) return FALSE;
	else if (len == 0) return TRUE;
	else return isSameString(bytes(), string.bytes(), len);
}

Bool StringSpan::operator !=(String string)
{
	UInt targetLength = string.length();
	if (len != targetLength) return TRUE;
	else if (len == 0) return FALSE;
	else return !isSameString(bytes(), string.bytes(), len);
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
		else return compareString(bytes(), len, span.bytes(), span.len) > 0;
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
		else return compareString(bytes(), len, span.bytes(), span.len) < 0;
	}
}

const Byte *StringSpan::bytes()
{
	return (Byte*)(source + idx);
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
	if (source) return StringCommon::getHashCode((Byte*)(source + idx), len);
	else return 0;
}

String StringSpan::clone()
{
	if (source) return StringCommon::clone((Byte*)(source + idx), len);
	else return String();
}

Array<Char> StringSpan::chars()
{
	return StringCommon::chars((Byte*)(source + idx), len);
}

Array<WChar> StringSpan::wchars()
{
	return StringCommon::wchars((Byte*)(source + idx), len);
}

StringAppender StringSpan::operator +(String string)
{
	return StringCommon::operatorPlus((Byte*)(source + idx), len, string);
}

Optional<Int> StringSpan::toInt()
{
	if (source) return StringCommon::toInt((Byte*)(source + idx), len);
	else return Optional<Int>();
}

Optional<Long> StringSpan::toLong()
{
	if (source) return StringCommon::toLong((Byte*)(source + idx), len);
	else return Optional<Long>();
}

Optional<Float> StringSpan::toFloat()
{
	if (source) return StringCommon::toFloat((Byte*)(source + idx), len);
	else return Optional<Float>();
}

Optional<Double> StringSpan::toDouble()
{
	if (source) return StringCommon::toDouble((Byte*)(source + idx), len);
	else return Optional<Double>();
}

Bool StringSpan::toNumber(Int& number)
{
	if (source) return StringCommon::toNumber((Byte*)(source + idx), len, number);
	else return FALSE;
}

Bool StringSpan::toNumber(Long& number)
{
	if (source) return StringCommon::toNumber((Byte*)(source + idx), len, number);
	else return FALSE;
}

Bool StringSpan::toNumber(Float& number)
{
	if (source) return StringCommon::toNumber((Byte*)(source + idx), len, number);
	else return FALSE;
}

Bool StringSpan::toNumber(Double& number)
{
	if (source) return StringCommon::toNumber((Byte*)(source + idx), len, number);
	else return FALSE;
}

Binary StringSpan::toBinary()
{
	if (source) return StringCommon::toBinary((Byte*)(source + idx), len);
	else return Binary();
}

String StringSpan::toUpper()
{
	if (source) return StringCommon::toUpper((Byte*)(source + idx), len);
	else return String();
}

String StringSpan::toLower()
{
	if (source) return StringCommon::toLower((Byte*)(source + idx), len);
	else return String();
}

Bool StringSpan::startsWith(String target)
{
	if (source) return StringCommon::startsWith((Byte*)(source + idx), len, target);
	else return FALSE;
}

Bool StringSpan::endsWith(String target)
{
	if (source) return StringCommon::endsWith((Byte*)(source + idx), len, target);
	else return FALSE;
}

Array<UInt> StringSpan::search(String target)
{
	if (source) return StringCommon::search((Byte*)(source + idx), len, target);
	else return Array<UInt>();
}

UInt StringSpan::searchFirst(String target)
{
	if (source) return StringCommon::searchFirst((Byte*)(source + idx), len, target);
	else return UINF;
}

UInt StringSpan::searchLast(String target)
{
	if (source) return StringCommon::searchLast((Byte*)(source + idx), len, target);
	else return UINF;
}

Array<StringSpan> StringSpan::split(String target)
{
	if (source) return StringCommon::split((Byte*)(source + idx), len, target, idx ? (Vars*)source : NULL);
	else return Array<StringSpan>();
}

String StringSpan::replace(String oldString, String newString)
{
	return StringCommon::replace((Byte*)(source + idx), len, oldString, newString);
}

StringSpan StringSpan::span(UInt index, UInt length)
{
	return StringSpan(*this, index, length);
}