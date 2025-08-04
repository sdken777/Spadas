
#include "binary.h"

using namespace spadas;
using namespace binary_internal;

String spadas::Binary::typeName() { static String name = "spadas.Binary"; return name; }

BinaryVars* Binary::var() { return (BinaryVars*)this->vars; }

Binary::Binary()
{}

Binary::Binary(UInt size)
{
	if (size == 0) return;
	size = math::min(size, ARRAY_SIZE_LIMIT);
	Byte* newVarsRaw = new Byte[sizeof(BinaryVars) + size];
	BinaryVars* newVars = new (newVarsRaw)BinaryVars(size, &newVarsRaw[sizeof(BinaryVars)]);
	setVars(newVars, TRUE);
}

Binary::Binary(UInt size, Byte origin)
{
	if (size == 0) return;

	size = math::min(size, ARRAY_SIZE_LIMIT);
	Byte* newVarsRaw = new Byte[sizeof(BinaryVars) + size];
	BinaryVars* newVars = new (newVarsRaw)BinaryVars(size, &newVarsRaw[sizeof(BinaryVars)]);
	setVars(newVars, TRUE);

	utility::memorySet(origin, var()->data, size);
}

Binary::Binary(const Byte *arr, UInt size)
{
	if (arr == NULL || size == 0) return;

	size = math::min(size, ARRAY_SIZE_LIMIT);
	Byte* newVarsRaw = new Byte[sizeof(BinaryVars) + size];
	BinaryVars* newVars = new (newVarsRaw)BinaryVars(size, &newVarsRaw[sizeof(BinaryVars)]);
	setVars(newVars, TRUE);

	utility::memoryCopy(arr, var()->data, size);
}

Binary Binary::create(UInt size, UInt firstByte, ...)
{
	if (size == 0) return Binary();
	Binary out(size);
	Byte *data = out.var()->data;
	data[0] = firstByte;
	if (size == 1) return out;
	va_list list;
	va_start(list, firstByte);
	for (UInt i = 1; i < size; i++)
	{
		data[i] = (Byte)va_arg(list, UInt);
	}
	va_end(list);
	return out;
}

Byte *Binary::data()
{
	return vars ? var()->data : 0;
}

UInt Binary::size()
{
	return vars ? var()->size : 0;
}

Bool Binary::isEmpty()
{
	return vars == 0;
}

Binary Binary::clone()
{
	if (!vars) return Binary();
	else return BinaryCommon::clone(var()->data, var()->size);
}

Byte& Binary::operator [](UInt index)
{
	SPADAS_ERROR_RETURNVAL(!vars || index >= var()->size, *(new Byte));
	return var()->data[index];
}

Binary Binary::operator +(Binary bin)
{
	if (!vars) return bin.clone();
	else return BinaryCommon::operatorPlus(var()->data, var()->size, bin);
}

Binary& Binary::trim(UInt size)
{
	SPADAS_ERROR_RETURNVAL(size == 0, *this);
	if (vars && size < var()->size)
	{
		var()->size = size;
	}
	return *this;
}

void Binary::set(Byte val)
{
	if (!vars) return;
	utility::memorySet(val, var()->data, var()->size);
}

Array<BinarySpan> Binary::split(Array<UInt> sizes)
{
	SPADAS_ERROR_RETURNVAL(!vars, Array<BinarySpan>());
	return BinaryCommon::split(var()->data, var()->size, sizes, vars);
}

Binary Binary::reverse()
{
	if (!vars) return Binary();
	else return BinaryCommon::reverse(var()->data, var()->size);
}

String Binary::toBase64()
{
	if (!vars) return String();
	else return BinaryCommon::toBase64(var()->data, var()->size);
}

Binary Binary::toSHA1()
{
	if (!vars) return Binary();
	else return BinaryCommon::toSHA1(var()->data, var()->size);
}

Binary Binary::toDES(String key)
{
	if (!vars) return Binary();
	else return BinaryCommon::toDES(var()->data, var()->size, key);
}

BinarySpan Binary::span(UInt index, UInt size)
{
	return BinarySpan(*this, index, size);
}

void Binary::copy(Binary src, Region srcRegion, UInt thisOffset)
{
	SPADAS_ERROR_RETURN(!vars || !src.vars || vars == src.vars);
	UInt srcSize = src.var()->size;
	SPADAS_ERROR_RETURN(srcRegion.offset < 0 || srcRegion.offset + srcRegion.size > srcSize);
	SPADAS_ERROR_RETURN(thisOffset + srcRegion.size > var()->size);
	utility::memoryCopy(&src.var()->data[srcRegion.offset], &var()->data[thisOffset], srcRegion.size);
}

Binary Binary::merge(Array<Binary> binaries)
{
	UInt totalSize = 0;
	for (UInt i = 0; i < binaries.size(); i++) totalSize += binaries[i].size();
	if (totalSize == 0) return Binary();
	SPADAS_ERROR_RETURNVAL(totalSize > ARRAY_SIZE_LIMIT, Binary());
	Binary out(totalSize);
	Byte *dstData = out.data();
	UInt n = 0;
	for (UInt i = 0; i < binaries.size(); i++)
	{
		Binary& bin = binaries[i];
		if (bin.isEmpty()) continue;
		UInt copySize = bin.var()->size;
		utility::memoryCopy(bin.var()->data, &dstData[n], copySize);
		n += copySize;
	}
	return out;
}

