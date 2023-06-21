#include "binary.h"

using namespace binary_internal;

Binary BinaryCommon::clone(const Byte* bytes, UInt size)
{
	Binary out(size);
	utility::memoryCopy(bytes, out.data(), size);
    return out;
}

Binary BinaryCommon::operatorPlus(const Byte* bytes, UInt size, Binary& append)
{
    UInt appendSize = append.size();
    Binary out(size + appendSize);
    Byte *outData = out.data();
    if (size > 0) utility::memoryCopy(bytes, outData, size);
    if (appendSize > 0) utility::memoryCopy(append.data(), &outData[size], appendSize);
    return out;
}

Array<BinarySpan> BinaryCommon::split(Binary& source, UInt spanIndex, UInt spanSize, Array<UInt>& sizes)
{
    UInt totalSize = 0;
    for (UInt i = 0; i < sizes.size(); i++) totalSize += sizes[i];
	SPADAS_ERROR_RETURNVAL(totalSize != spanSize, Array<BinarySpan>());
    Array<BinarySpan> out = Array<BinarySpan>::createUninitialized(sizes.size());
    UInt curIndex = spanIndex;
    for (UInt i = 0; i < sizes.size(); i++)
    {
        UInt subArraySize = sizes[i];
		if (subArraySize == 0)
        {
            out.initialize(i, BinarySpan());
        }
        else
        {
            out.initialize(i, BinarySpan(source, curIndex, subArraySize));
            curIndex += subArraySize;
        }
    }
    return out;
}

Binary BinaryCommon::reverse(const Byte* bytes, UInt size)
{
	Binary out(size);
	Byte *dst = out.data();
	for (Int i = 0; i < (Int)size; i++)
	{
		dst[i] = bytes[size - i - 1];
	}
	return out;
}

String BinaryCommon::toBase64(const Byte* bytes, UInt size)
{
	Char *base64EncodedChars = base64Encode((Char*)bytes, size);
	if (base64EncodedChars == NULL) return String();

	String result = base64EncodedChars;
	delete[] base64EncodedChars;
	return result;
}

Binary BinaryCommon::toSHA1(const Byte* bytes, UInt size)
{
	Binary encoded(20);
	calc_sha1(bytes, size, encoded.data());
	return encoded;
}

Binary BinaryCommon::toDES(const Byte* bytes, UInt size, String& key)
{
	if (key.isEmpty()) key = "spadas";
	Binary sha1 = key.toBinary().toSHA1();
	return desEncode(sha1, bytes, size);
}

BinarySpan BinaryCommon::sub(Binary& source, UInt spanIndex, UInt spanSize, UInt subIndex, UInt subSize)
{
    SPADAS_ERROR_RETURNVAL(subIndex >= spanSize, BinarySpan());
    if (subSize == 0) return BinarySpan();

    subSize = math::min(subSize, spanSize - subIndex);
    return BinarySpan(source, spanIndex + subIndex, subSize);
}