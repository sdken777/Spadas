#include "binary.h"

using namespace binary_internal;

BinarySpan::BinarySpan() : source(0), idx(0), siz(0)
{}

BinarySpan::BinarySpan(const Byte *ptr, UInt size) : source(0), idx(0), siz(0)
{
    if (size == 0) return;

    SPADAS_ERROR_RETURN(ptr == NULL);
    source = (ULong)ptr;
    siz = size;
}

BinarySpan::BinarySpan(Binary& sourceBinary, UInt offset, UInt size) : source(0), idx(0), siz(0)
{
    if (size == 0) return;

    BinaryVars *sourceVars = sourceBinary.var();
    SPADAS_ERROR_RETURN(!sourceVars);
    SPADAS_ERROR_RETURN(offset >= sourceVars->size);
    sourceVars->retain();
    source = (ULong)sourceVars;
    idx = (UInt)((ULong)sourceVars->data + offset - source);
    siz = math::min(size, sourceVars->size - offset);
}

BinarySpan::BinarySpan(BinarySpan& sourceSpan, UInt offset, UInt size) : source(0), idx(0), siz(0)
{
    if (size == 0) return;

    SPADAS_ERROR_RETURN(sourceSpan.source == 0);
    SPADAS_ERROR_RETURN(offset >= sourceSpan.siz);
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
    siz = math::min(size, sourceSpan.siz - offset);
}

BinarySpan::BinarySpan(const BinarySpan& span) : source(span.source), idx(span.idx), siz(span.siz)
{
    if (idx) ((Vars*)source)->retain();
}

BinarySpan::~BinarySpan()
{
    if (idx) ((Vars*)source)->release();
}

BinarySpan& BinarySpan::operator =(const BinarySpan& span)
{
    if (idx) ((Vars*)source)->release();
    source = span.source;
    idx = span.idx;
    siz = span.siz;
    if (idx) ((Vars*)source)->retain();
    return *this;
}

Bool BinarySpan::isRefCounting()
{
    return idx != 0;
}

Byte* BinarySpan::data()
{
    return (Byte*)(source + idx);
}

UInt BinarySpan::size()
{
    return siz;
}

Bool BinarySpan::isEmpty()
{
    return siz == 0;
}

Binary BinarySpan::clone()
{
    if (source)
    {
        Binary output(siz);
        utility::memoryCopy((Byte*)(source + idx), output.data(), siz);
        return output;
    }
    else return Binary();
}

Byte& BinarySpan::operator [](UInt index)
{
    SPADAS_ERROR_RETURNVAL(index >= siz, *(new Byte));
    return *(Byte*)(source + idx + index);
}

Binary BinarySpan::operator +(Binary bin)
{
    if (source) return BinaryCommon::operatorPlus((Byte*)(source + idx), siz, bin);
	else return bin.clone();
}

BinarySpan& BinarySpan::trim(UInt size)
{
    SPADAS_ERROR_RETURNVAL(size == 0, *this);
    siz = math::min(siz, size);
    return *this;
}

void BinarySpan::set(Byte val)
{
    if (source) utility::memorySet(val, (Byte*)(source + idx), siz);
}

Array<BinarySpan> BinarySpan::split(Array<UInt> sizes)
{
	SPADAS_ERROR_RETURNVAL(!source, Array<BinarySpan>());
	return BinaryCommon::split((Byte*)(source + idx), siz, sizes, idx ? (Vars*)source : NULL);
}

Binary BinarySpan::reverse()
{
    if (source) return BinaryCommon::reverse((Byte*)(source + idx), siz);
    else return Binary();
}

String BinarySpan::toBase64()
{
    if (source) return BinaryCommon::toBase64((Byte*)(source + idx), siz);
    else return String();
}

Binary BinarySpan::toSHA1()
{
    if (source) return BinaryCommon::toSHA1((Byte*)(source + idx), siz);
    else return Binary();
}

Binary BinarySpan::toDES(String key)
{
    if (source) return BinaryCommon::toDES((Byte*)(source + idx), siz, key);
    else return Binary();
}

BinarySpan BinarySpan::span(UInt index, UInt size)
{
    return BinarySpan(*this, index, size);
}