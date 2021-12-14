
#if defined(SPADAS_DEBUG)
#undef SPADAS_DEBUG
#endif

#include "spadas.h"
#include "binary.h"

#if defined(SPADAS_ENV_MACOS)
#pragma clang diagnostic ignored "-Wvarargs"
#endif

using namespace spadas;

const String spadas::Binary::TypeName = "spadas.Binary";

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

	utility::memorySet(origin, vars->data, size);
}

Binary::Binary(Byte *arr, UInt size)
{
	if (arr == NULL || size == 0) return;

	size = math::min(size, ARRAY_SIZE_LIMIT);
	Byte* newVarsRaw = new Byte[sizeof(BinaryVars) + size];
	BinaryVars* newVars = new (newVarsRaw)BinaryVars(size, &newVarsRaw[sizeof(BinaryVars)]);
	setVars(newVars, TRUE);

	utility::memoryCopy(arr, vars->data, size);
}

Binary::Binary(Binary input, Region region)
{
	if (input.isEmpty()) return;

	UInt inputSize = input.size();
	Int startIndex = region.offset, endIndex = (region.size == UINF ? INF : (region.offset + (Int)region.size - 1));
	if (region.size == 0 || startIndex >= (Int)inputSize || endIndex < 0) return;

	startIndex = math::max(0, startIndex);
	endIndex = math::min((Int)inputSize - 1, endIndex);
	Int size = endIndex + 1 - startIndex;
	if (size <= 0) return;

	size = math::min((UInt)size, ARRAY_SIZE_LIMIT);
	Byte* newVarsRaw = new Byte[sizeof(BinaryVars) + size];
	BinaryVars* newVars = new (newVarsRaw)BinaryVars(size, &newVarsRaw[sizeof(BinaryVars)]);
	setVars(newVars, TRUE);

	utility::memoryCopy(&input.vars->data[startIndex], vars->data, size);
}

Binary Binary::create(UInt size, Byte firstByte, ...)
{
	if (size == 0) return Binary();
	Binary out(size);
	Byte *data = out.vars->data;
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

UInt Binary::size()
{
	return vars ? vars->size : 0;
}

Byte *Binary::data()
{
	return vars ? vars->data : 0;
}

Byte& Binary::operator [](UInt index)
{
	SPADAS_ERROR_RETURNVAL(!vars || index >= vars->size, *(new Byte));
	return vars->data[index];
}

Binary Binary::subBinary(UInt index, UInt size)
{
	SPADAS_ERROR_RETURNVAL(!vars || index >= vars->size, Binary());
	size = math::min(size, vars->size - index);
	if (size == 0) return Binary();
	Binary subbin;
	subbin.setVars(new BinaryVars(size, &this->vars->data[index], this->vars), TRUE);
	return subbin;
}

Bool Binary::isEmpty()
{
	return vars == 0;
}

void Binary::trim(UInt size)
{
	if (vars && size < vars->size)
	{
		if (size == 0) setVars(0, FALSE);
		else vars->size = size;
	}
}

Binary Binary::clone()
{
	if (!vars) return Binary();
	Binary out(vars->size);
	utility::memoryCopy(vars->data, out.vars->data, vars->size);
    return out;
}

void Binary::clear(Byte val)
{
	if (!vars) return;
	utility::memorySet(val, vars->data, vars->size);
}

void Binary::copy(Binary src, Region srcRegion, UInt thisOffset)
{
	SPADAS_ERROR_RETURN(!vars || !src.vars || vars == src.vars);
	UInt srcSize = src.vars->size;
	SPADAS_ERROR_RETURN(srcRegion.offset < 0 || srcRegion.offset + srcRegion.size > srcSize);
	SPADAS_ERROR_RETURN(thisOffset + srcRegion.size > vars->size);
	utility::memoryCopy(&src.vars->data[srcRegion.offset], &vars->data[thisOffset], srcRegion.size);
}

Array<Binary> Binary::split(Array<UInt> sizes)
{
	if (!vars) return Array<Binary>();
    UInt totalSize = 0;
    for (UInt i = 0; i < sizes.size(); i++) totalSize += sizes[i];
	SPADAS_ERROR_RETURNVAL(totalSize != vars->size, Array<Binary>());
    Array<Binary> out(sizes.size());
    UInt n = 0;
    for (UInt i = 0; i < sizes.size(); i++)
    {
        UInt subArraySize = sizes[i];
		if (subArraySize == 0) continue;
        out[i] = Binary(&vars->data[n], subArraySize);
		n += subArraySize;
    }
    return out;
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
		UInt copySize = bin.vars->size;
		utility::memoryCopy(bin.vars->data, &dstData[n], copySize);
		n += copySize;
	}
	return out;
}

Binary Binary::operator +(Binary bin)
{
	if (vars)
	{
		if (!bin.vars) return clone();
		else
		{
			UInt thisSize = vars->size, binSize = bin.vars->size;
			Binary out(thisSize + binSize);
			Byte *outData = out.vars->data;
			utility::memoryCopy(vars->data, outData, thisSize);
			utility::memoryCopy(bin.vars->data, &outData[thisSize], binSize);
			return out;
		}
	}
	else return bin.clone();
}

Binary Binary::reverse()
{
	if (!vars) return Binary();

	UInt size = vars->size;
	Binary out(size);
	Byte *src = vars->data;
	Byte *dst = out.vars->data;
	for (Int i = 0; i < (Int)size; i++)
	{
		dst[i] = src[size - i - 1];
	}
	return out;
}