
#include "string_spadas.h"

using namespace spadas;

StringBuffer::StringBuffer() : BaseObject(new StringBufferVars)
{
	var()->target = String::createWithSize(1024);
}

StringBuffer::StringBuffer(UInt bufferSize) : BaseObject(new StringBufferVars)
{
	var()->target = String::createWithSize(math::max(1024u, bufferSize));
}

void StringBuffer::reset()
{
	StringVars *stringVars = var()->target.var();
	stringVars->length = 0;
}

void StringBuffer::append(Char c)
{
	StringVars *stringVars = var()->target.var();
	if (stringVars->length == stringVars->size)
	{
		UInt newBufferSize = stringVars->length * 2;
		Byte *dstData = new Byte[newBufferSize];
		utility::memoryCopy(stringVars->data, dstData, stringVars->length);
		if (stringVars->extData) delete[] stringVars->extData;
		stringVars->data = stringVars->extData = dstData;
		stringVars->size = newBufferSize;
	}
	stringVars->data[stringVars->length++] = (Byte)c;
}

String StringBuffer::output(Bool clone)
{
    if (clone) return var()->target.clone();
    else return var()->target;
}