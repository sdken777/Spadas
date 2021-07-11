
#include "spadas.h"

#include "string_bz.h"

namespace string_internal
{
	using namespace spadas;

	Char hexCharacter(UInt value)
	{
		switch (value)
		{
			case 0:
				return '0';
			case 1:
				return '1';
			case 2:
				return '2';
			case 3:
				return '3';
			case 4:
				return '4';
			case 5:
				return '5';
			case 6:
				return '6';
			case 7:
				return '7';
			case 8:
				return '8';
			case 9:
				return '9';
			case 10:
				return 'A';
			case 11:
				return 'B';
			case 12:
				return 'C';
			case 13:
				return 'D';
			case 14:
				return 'E';
			case 15:
				return 'F';
		}
		return '\0';
	}
}

using namespace spadas;
using namespace string_internal;

String String::createWithSize(UInt size)
{
	if (size == 0) return String();
	String out;
	out.setVars(new StringVars(size), TRUE);
	return out;
}

String String::createHexString(Byte value)
{
	Char buf[3];
	
	buf[0] = hexCharacter((UInt)((value & 0xf0) >> 4));
	buf[1] = hexCharacter((UInt)((value & 0x0f) >> 0));
	buf[2] = 0;
	
	return buf;
}

String String::createHexString(Word value)
{
	Char buf[5];

	buf[0] = hexCharacter((UInt)((value & 0xf000) >> 12));
	buf[1] = hexCharacter((UInt)((value & 0x0f00) >> 8));
	buf[2] = hexCharacter((UInt)((value & 0x00f0) >> 4));
	buf[3] = hexCharacter((UInt)((value & 0x000f) >> 0));
	buf[4] = 0;

	return buf;
}

String String::createHexString(UInt value)
{
	Char buf[9];
	
	buf[0] = hexCharacter((value & 0xf0000000) >> 28);
	buf[1] = hexCharacter((value & 0x0f000000) >> 24);
	buf[2] = hexCharacter((value & 0x00f00000) >> 20);
	buf[3] = hexCharacter((value & 0x000f0000) >> 16);
	buf[4] = hexCharacter((value & 0x0000f000) >> 12);
	buf[5] = hexCharacter((value & 0x00000f00) >> 8);
	buf[6] = hexCharacter((value & 0x000000f0) >> 4);
	buf[7] = hexCharacter((value & 0x0000000f) >> 0);
	buf[8] = 0;
	
	return buf;
}

String String::createHexString(ULong value)
{
	return String::createHexString((UInt)(value >> 32)) +
		String::createHexString((UInt)value);
}

String String::createBinString(Byte value)
{
	Char buf[9];

	for (UInt i = 0; i < 8; i++)
	{
		buf[i] = (((UInt)value << (24 + i)) >> 31) == 0 ? '0' : '1';
	}
	buf[8] = 0;

	return buf;
}

String String::createBinString(Word value)
{
	Char buf[17];

	for (UInt i = 0; i < 16; i++)
	{
		buf[i] = (((UInt)value << (16 + i)) >> 31) == 0 ? '0' : '1';
	}
	buf[16] = 0;

	return buf;
}

String String::createBinString(UInt value)
{
	Char buf[33];

	for (UInt i = 0; i < 32; i++)
	{
		buf[i] = ((value << i) >> 31) == 0 ? '0' : '1';
	}
	buf[32] = 0;

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
	String out = String::createWithSize(3 * binSize + nRows + 1);
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
	size += (strs.size() - 1) * separator.length() + 1;
	
	String out = String::createWithSize(size);
	for (UInt i = 0; i < strs.size(); i++)
	{
		if (i != 0) out += separator;
		out += strs[i];
	}
	return out;
}