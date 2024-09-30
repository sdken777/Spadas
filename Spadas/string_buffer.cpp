
#include "string_spadas.h"

using namespace spadas;

StringBuffer::StringBuffer() : Object<StringBufferVars>(new StringBufferVars, TRUE)
{
	vars->target = String::createWithSize(1024);
}

StringBuffer::StringBuffer(UInt bufferSize) : Object<StringBufferVars>(new StringBufferVars, TRUE)
{
	vars->target = String::createWithSize(math::max(1024u, bufferSize));
}

void StringBuffer::reset()
{
	StringVars *stringVars = vars->target.getVars();
	stringVars->length = 0;
}

void StringBuffer::append(Char c)
{
	StringVars *stringVars = vars->target.getVars();
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
    if (clone) return vars->target.clone();
    else return vars->target;
}