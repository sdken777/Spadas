
#include "spadas.h"
#include "binary.h"

namespace spadas
{
	const UInt BINARY_FACTORY_DEFAULT_BYTES = 8;
	const UInt BINARY_FACTORY_BLOCK_SIZE = 64; // 每个block可生成多少个Binary

	class BinaryFactoryVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(BinaryFactory, Vars)

		UInt binaryBytes;
		Binary curBlock;
		UInt curIndex;

		BinaryFactoryVars(UInt binaryBytes0) : binaryBytes(binaryBytes0), curBlock(binaryBytes0 * BINARY_FACTORY_BLOCK_SIZE), curIndex(0)
		{}
	};
}

using namespace spadas;

const String spadas::BinaryFactory::TypeName = "spadas.BinaryFactory";

BinaryFactory::BinaryFactory() : Object<BinaryFactoryVars>(new BinaryFactoryVars(BINARY_FACTORY_DEFAULT_BYTES), TRUE)
{
}

BinaryFactory::BinaryFactory(UInt binaryBytes)
{
	binaryBytes = (math::clamp(binaryBytes, 4u, 4096u) + 3) & 0xfffc;
	setVars(new BinaryFactoryVars(binaryBytes), TRUE);
}

Binary BinaryFactory::create(UInt trimSize)
{
	Binary output = vars->curBlock.subBinary(vars->curIndex++ * vars->binaryBytes, math::min(trimSize, vars->binaryBytes));
	if (vars->curIndex >= BINARY_FACTORY_BLOCK_SIZE)
	{
		vars->curBlock = Binary(vars->binaryBytes * BINARY_FACTORY_BLOCK_SIZE);
		vars->curIndex = 0;
	}
	return output;
}

Binary BinaryFactory::create(UInt trimSize, Byte origin)
{
	Binary output = vars->curBlock.subBinary(vars->curIndex++ * vars->binaryBytes, math::min(trimSize, vars->binaryBytes));
	if (vars->curIndex >= BINARY_FACTORY_BLOCK_SIZE)
	{
		vars->curBlock = Binary(vars->binaryBytes * BINARY_FACTORY_BLOCK_SIZE);
		vars->curIndex = 0;
	}

	Byte* outputData = output.data();
	UInt outputSize = output.size();
	if (outputSize < MEMCPY_THRESH)
	{
		for (UInt i = 0; i < outputSize; i++) outputData[i] = origin;
	}
	else
	{
		utility::memorySet(origin, outputData, outputSize);
	}

	return output;
}
