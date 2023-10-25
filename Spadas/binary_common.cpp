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

Array<BinarySpan> BinaryCommon::split(const Byte* bytes, UInt size, Array<UInt>& sizes, Vars *binaryVars)
{
    UInt totalSize = 0;
    for (UInt i = 0; i < sizes.size(); i++) totalSize += sizes[i];
	SPADAS_ERROR_RETURNVAL(totalSize != size, Array<BinarySpan>());

    Binary sourceBinary;
    UInt curIndex = 0;
    if (binaryVars)
    {
        auto objBase = Object<BinaryVars>::castCreate(binaryVars);
		sourceBinary = *(Binary*)(&objBase);
        curIndex = (UInt)(bytes - sourceBinary.data());
    }
    
    Array<BinarySpan> out = Array<BinarySpan>::createUninitialized(sizes.size());
    for (UInt i = 0; i < sizes.size(); i++)
    {
        UInt subArraySize = sizes[i];
		if (subArraySize == 0)
        {
            out.initialize(i, BinarySpan());
        }
        else
        {
            out.initialize(i, binaryVars ? BinarySpan(sourceBinary, curIndex, subArraySize) : BinarySpan(bytes + curIndex, subArraySize));
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