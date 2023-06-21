#include "binary.h"

using namespace binary_internal;

BinarySpan::BinarySpan() : idx(0), siz(0)
{}

BinarySpan::BinarySpan(Binary& sourceBinary, UInt index, UInt size) : source(sourceBinary), idx(index), siz(size)
{}

Byte* BinarySpan::data()
{
    return siz == 0 ? 0 : (source.data() + idx);
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
    if (!siz) return Binary();
    else return Binary(source.data() + idx, siz);
}

Byte& BinarySpan::operator [](UInt index)
{
    SPADAS_ERROR_RETURNVAL(index >= siz, *(new Byte));
    return source.data()[idx + index];
}

Binary BinarySpan::operator +(Binary bin)
{
	if (!siz) return bin.clone();
	else return BinaryCommon::operatorPlus(source.data() + idx, siz, bin);
}

void BinarySpan::trim(UInt size)
{
    SPADAS_ERROR_RETURN(size == 0);
    if (size < siz) siz = size;
}

void BinarySpan::set(Byte val)
{
    if (!siz) return;
    utility::memorySet(val, source.data() + idx, siz);
}

Array<BinarySpan> BinarySpan::split(Array<UInt> sizes)
{
	SPADAS_ERROR_RETURNVAL(siz == 0, Array<BinarySpan>());
	return BinaryCommon::split(source, idx, siz, sizes);
}

Binary BinarySpan::reverse()
{
    if (!siz) return Binary();
    return BinaryCommon::reverse(source.data() + idx, siz);
}

String BinarySpan::toBase64()
{
    if (!siz) return String();
    return BinaryCommon::toBase64(source.data() + idx, siz);
}

Binary BinarySpan::toSHA1()
{
    if (!siz) return Binary();
    return BinaryCommon::toSHA1(source.data() + idx, siz);
}

Binary BinarySpan::toDES(String key)
{
    if (!siz) return Binary();
    return BinaryCommon::toDES(source.data() + idx, siz, key);
}

BinarySpan BinarySpan::sub(UInt index, UInt size)
{
	SPADAS_ERROR_RETURNVAL(siz == 0, BinarySpan());
    return BinaryCommon::sub(source, idx, siz, index, size);
}