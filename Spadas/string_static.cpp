
#include "spadas.h"
#include "string_spadas.h"

namespace string_internal
{
	using namespace spadas;

	Byte hexCharacter(UInt value)
	{
		switch (value)
		{
			case 0:
				return (Byte)'0';
			case 1:
				return (Byte)'1';
			case 2:
				return (Byte)'2';
			case 3:
				return (Byte)'3';
			case 4:
				return (Byte)'4';
			case 5:
				return (Byte)'5';
			case 6:
				return (Byte)'6';
			case 7:
				return (Byte)'7';
			case 8:
				return (Byte)'8';
			case 9:
				return (Byte)'9';
			case 10:
				return (Byte)'A';
			case 11:
				return (Byte)'B';
			case 12:
				return (Byte)'C';
			case 13:
				return (Byte)'D';
			case 14:
				return (Byte)'E';
			case 15:
				return (Byte)'F';
		}
		return 0;
	}
}

using namespace spadas;
using namespace string_internal;

String String::createWithSize(UInt size)
{
	if (size == 0) return String();

	Byte* newVarsRaw = new Byte[sizeof(StringVars) + size];
	StringVars* newVars = new (newVarsRaw)StringVars(size, &newVarsRaw[sizeof(StringVars)]);
	newVars->length = 0;

	String out;
	out.setVars(newVars, TRUE);
	return out;
}

String String::createHexString(Byte value)
{
	Binary buf(2);
	
	buf[0] = hexCharacter((UInt)((value & 0xf0) >> 4));
	buf[1] = hexCharacter((UInt)((value & 0x0f) >> 0));
	
	return buf;
}

String String::createHexString(Word value)
{
	Binary buf(4);

	buf[0] = hexCharacter((UInt)((value & 0xf000) >> 12));
	buf[1] = hexCharacter((UInt)((value & 0x0f00) >> 8));
	buf[2] = hexCharacter((UInt)((value & 0x00f0) >> 4));
	buf[3] = hexCharacter((UInt)((value & 0x000f) >> 0));

	return buf;
}

String String::createHexString(UInt value)
{
	Binary buf(8);
	
	buf[0] = hexCharacter((value & 0xf0000000) >> 28);
	buf[1] = hexCharacter((value & 0x0f000000) >> 24);
	buf[2] = hexCharacter((value & 0x00f00000) >> 20);
	buf[3] = hexCharacter((value & 0x000f0000) >> 16);
	buf[4] = hexCharacter((value & 0x0000f000) >> 12);
	buf[5] = hexCharacter((value & 0x00000f00) >> 8);
	buf[6] = hexCharacter((value & 0x000000f0) >> 4);
	buf[7] = hexCharacter((value & 0x0000000f) >> 0);
	
	return buf;
}

String String::createHexString(ULong value)
{
	return String::createHexString((UInt)(value >> 32)) +
		String::createHexString((UInt)value);
}

String String::createBinString(Byte value)
{
	Binary buf(8);

	for (UInt i = 0; i < 8; i++)
	{
		buf[i] = (((UInt)value << (24 + i)) >> 31) == 0 ? (Byte)'0' : (Byte)'1';
	}

	return buf;
}

String String::createBinString(Word value)
{
	Binary buf(16);

	for (UInt i = 0; i < 16; i++)
	{
		buf[i] = (((UInt)value << (16 + i)) >> 31) == 0 ? (Byte)'0' : (Byte)'1';
	}

	return buf;
}

String String::createBinString(UInt value)
{
	Binary buf(32);

	for (UInt i = 0; i < 32; i++)
	{
		buf[i] = ((value << i) >> 31) == 0 ? (Byte)'0' : (Byte)'1';
	}

	return buf;
}

String String::createBinString(ULong value)
{
	return String::createBinString((UInt)(value >> 32)) +
		String::createBinString((UInt)value);
}

String String::createHexString(Binary bin, UInt nBytesPerRow)
{
	if (bin.isEmpty()) return String();
	
	UInt binSize = bin.size();
	Byte *binData = bin.data();
	
	nBytesPerRow = math::clamp(nBytesPerRow, 1u, bin.size());
	UInt nRows = (binSize + nBytesPerRow - 1) / nBytesPerRow;

	String space = " ";
	String enter = "\n";
	String out = String::createWithSize(3 * binSize + nRows);
	for (UInt i = 0; i < nRows - 1; i++)
	{
		for (UInt j = 0; j < nBytesPerRow; j++)
		{
			out += String::createHexString(binData[i * nBytesPerRow + j]);
			out += space;
		}
		out += enter;
	}
	for (UInt i = (nRows-1) * nBytesPerRow; i < binSize; i++)
	{
		out += String::createHexString(binData[i]);
		out += space;
	}
	return out;
}

String String::mergeStrings(Array<String> strs, String separator)
{
	if (strs.isEmpty()) return String();
	
	UInt size = 0;
	for (UInt i = 0; i < strs.size(); i++)
	{
		size += strs[i].length();
	}
	size += (strs.size() - 1) * separator.length();
	
	String out = String::createWithSize(size);
	for (UInt i = 0; i < strs.size(); i++)
	{
		if (i != 0) out += separator;
		out += strs[i];
	}
	return out;
}

String String::merge(Array<StringSpan> spans, String separator)
{
	if (spans.isEmpty()) return String();
	
	UInt size = 0;
	for (UInt i = 0; i < spans.size(); i++)
	{
		size += spans[i].length();
	}
	size += (spans.size() - 1) * separator.length();
	
	String out = String::createWithSize(size);
	for (UInt i = 0; i < spans.size(); i++)
	{
		if (i != 0) out += separator;
		out += spans[i];
	}
	return out;
}