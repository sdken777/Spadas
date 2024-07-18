
#include "string_spadas.h"

namespace string_internal
{
	ULong tenPowers[20];
	UInt tenPowersInited = FALSE;

	void initTenPowers()
	{
		if (tenPowersInited) return;
		for (UInt n = 0; n < 20; n++)
		{
			ULong coef = 1;
			for (UInt i = 0; i < n; i++) coef *= 10;
			tenPowers[n] = coef;
		}
		tenPowersInited = TRUE;
	}

	UInt lengthChar(const Char *chars)
	{
		if (!chars) return 0;
		ULong length = 0;
		while (TRUE)
		{
			if(chars[length] == 0) break;
			length++;
		}
		return length;
	}

	UInt lengthWChar(const WChar *chars)
	{
		if (!chars) return 0;
		ULong length = 0;
		while (TRUE)
		{
			if(chars[length] == 0) break;
			length++;
		}
		return length;
	}

	Bool isSameString(const Byte *data1, const Byte *data2, UInt length)
	{
		for (UInt i = 0; i < length; i++)
		{
			if (data1[i] != data2[i]) return FALSE;
		}
		return TRUE;
	}

	Int compareString(const Byte *data1, UInt length1, const Byte *data2, UInt length2)
	{
		UInt overlapLength = math::min(length1, length2);
		for (UInt i = 0; i < overlapLength; i++)
		{
			if (data1[i] == data2[i]) continue;
			return data1[i] > data2[i] ? 1 : -1;
		}
		if (length1 == length2) return 0;
		else return length1 > length2 ? 1 : -1;
	}

	UInt writeIntPart(Char *data, Bool negative, UInt intValPos, UInt& intDigits) // 写Float的符号位和整数部分
	{
		intDigits = 1;
		if (intValPos >= 100)
		{
			if (intValPos >= 10000)
			{
				if (intValPos >= 100000) intDigits = 6;
				else intDigits = 5;
			}
			else
			{
				if (intValPos >= 1000) intDigits = 4;
				else intDigits = 3;
			}
		}
		else
		{
			if (intValPos >= 10) intDigits = 2;
		}

		UInt curIndex = 0;
		if (negative) data[curIndex++] = '-';
		for (Int i = intDigits - 1; i >= 0; i--)
		{
			data[curIndex + i] = intValPos % 10 + 48;
			intValPos /= 10;
		}
		curIndex += intDigits;
		return curIndex;
	}

	UInt writeIntPart(Char *data, Bool negative, ULong intValPos, UInt& intDigits) // 写Double的符号位和整数部分
	{
		intDigits = 1;
		if (intValPos >= 10000ull)
		{
			if (intValPos >= 1000000ull)
			{
				if (intValPos >= 100000000ull)
				{
					if (intValPos >= 1000000000ull) intDigits = 10;
					else intDigits = 9;
				}
				else
				{
					if (intValPos >= 10000000ull) intDigits = 8;
					else intDigits = 7;
				}
			}
			else
			{
				if (intValPos >= 100000ull) intDigits = 6;
				else intDigits = 5;
			}
		}
		else
		{
			if (intValPos >= 100ull)
			{
				if (intValPos >= 1000ull) intDigits = 4;
				else intDigits = 3;
			}
			else
			{
				if (intValPos >= 10ull) intDigits = 2;
			}
		}

		UInt curIndex = 0;
		if (negative) data[curIndex++] = '-';
		for (Int i = intDigits - 1; i >= 0; i--)
		{
			data[curIndex + i] = intValPos % 10 + 48;
			intValPos /= 10;
		}
		curIndex += intDigits;
		return curIndex;
	}

	Int trimFloat(Char *floatData, Int floatDigits) // 返回0表示无小数部分，-1表示无小数部分但整数部分需要+1，>0表示实际的有效位数
	{
		Int firstNotZeroIndex = -1;
		for (Int i = 0; i < floatDigits; i++)
		{
			if (floatData[i] > '0')
			{
				firstNotZeroIndex = i;
				break;
			}
		}

		Int trimIndex = -1;
		Bool targetNine = FALSE;
		if (firstNotZeroIndex < 0)
		{
			trimIndex = 0;
		}
		else
		{
			for (Int i = firstNotZeroIndex; i <= floatDigits - 4; i++)
			{
				if (floatData[i] == '0' && floatData[i + 1] == '0' && floatData[i + 2] == '0' && floatData[i + 3] == '0')
				{
					trimIndex = i;
					break;
				}
				else if (floatData[i] == '9' && floatData[i + 1] == '9' && floatData[i + 2] == '9' && floatData[i + 3] == '9')
				{
					trimIndex = i;
					targetNine = TRUE;
					break;
				}
			}
		}

		if (trimIndex == -1)
		{
			Int trailZeroIndex = -1;
			for (Int i = floatDigits - 1; i >= 0; i--)
			{
				if (floatData[i] == '0') trailZeroIndex = i;
				else break;
			}
			return trailZeroIndex == -1 ? floatDigits : trailZeroIndex;
		}
		else if (trimIndex == 0)
		{
			return targetNine ? -1 : 0;
		}
		else
		{
			floatData[trimIndex - 1] += (Char)targetNine;
			return trimIndex;
		}
	}

	inline UInt writeBool(Bool val, Byte *data)
	{
		if (val)
		{
			data[0] = 'T';
			data[1] = 'R';
			data[2] = 'U';
			data[3] = 'E';
			return 4;
		}
		else
		{
			data[0] = 'F';
			data[1] = 'A';
			data[2] = 'L';
			data[3] = 'S';
			data[4] = 'E';
			return 5;
		}
	}

	inline UInt writeByte(Byte val, Byte *data)
	{
		Int lastIndex = 0;
		if (val >= 100) lastIndex = 2;
		else if (val >= 10) lastIndex = 1;

		for (Int i = lastIndex; i >= 0; i--)
		{
			data[i] = val % 10 + 48;
			val /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeWord(Word val, Byte *data)
	{
		Int lastIndex = 0;
		if (val >= 1000)
		{
			if (val >= 10000) lastIndex = 4;
			else lastIndex = 3;
		}
		else
		{
			if (val >= 100) lastIndex = 2;
			else if (val >= 10) lastIndex = 1;
		}

		for (Int i = lastIndex; i >= 0; i--)
		{
			data[i] = val % 10 + 48;
			val /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeUInt(UInt val, Byte *data)
	{
		Int lastIndex = 0;
		if (val >= 100000)
		{
			if (val >= 10000000)
			{
				if (val >= 1000000000) lastIndex = 9;
				else if (val >= 100000000) lastIndex = 8;
				else lastIndex = 7;
			}
			else
			{
				if (val >= 1000000) lastIndex = 6;
				else lastIndex = 5;
			}
		}
		else
		{
			if (val >= 1000)
			{
				if (val >= 10000) lastIndex = 4;
				else lastIndex = 3;
			}
			else
			{
				if (val >= 100) lastIndex = 2;
				else if (val >= 10) lastIndex = 1;
			}
		}

		for (Int i = lastIndex; i >= 0; i--)
		{
			data[i] = val % 10 + 48;
			val /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeULong(ULong val, Byte *data)
	{
		Int lastIndex = 0;
		if (val >= 10000000000ull)
		{
			if (val >= 1000000000000000ull)
			{
				if (val >= 100000000000000000ull)
				{
					if (val >= 10000000000000000000ull) lastIndex = 19;
					else if (val >= 1000000000000000000ull) lastIndex = 18;
					else lastIndex = 17;
				}
				else
				{
					if (val >= 10000000000000000ull) lastIndex = 16;
					else lastIndex = 15;
				}
			}
			else
			{
				if (val >= 10000000000000ull)
				{
					if (val >= 100000000000000ull) lastIndex = 14;
					else lastIndex = 13;
				}
				else
				{
					if (val >= 1000000000000ull) lastIndex = 12;
					else if (val >= 100000000000ull) lastIndex = 11;
					else lastIndex = 10;
				}
			}
		}
		else
		{
			if (val >= 100000ull)
			{
				if (val >= 10000000ull)
				{
					if (val >= 1000000000ull) lastIndex = 9;
					else if (val >= 100000000ull) lastIndex = 8;
					else lastIndex = 7;
				}
				else
				{
					if (val >= 1000000ull) lastIndex = 6;
					else lastIndex = 5;
				}
			}
			else
			{
				if (val >= 1000ull)
				{
					if (val >= 10000ull) lastIndex = 4;
					else lastIndex = 3;
				}
				else
				{
					if (val >= 100ull) lastIndex = 2;
					else if (val >= 10ull) lastIndex = 1;
				}
			}
		}

		for (Int i = lastIndex; i >= 0; i--)
		{
			data[i] = val % 10 + 48;
			val /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeShort(Short val, Byte *data)
	{
		Word valPos = (Word)math::abs(val);

		Int lastIndex = 0;
		if (valPos >= 1000)
		{
			if (valPos >= 10000) lastIndex = 4;
			else lastIndex = 3;
		}
		else
		{
			if (valPos >= 100) lastIndex = 2;
			else if (valPos >= 10) lastIndex = 1;
		}

		Int lowerBound = 0;
		if (val < 0)
		{
			data[0] = (Byte)'-';
			lastIndex++;
			lowerBound++;
		}

		for (Int i = lastIndex; i >= lowerBound; i--)
		{
			data[i] = valPos % 10 + 48;
			valPos /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeInt(Int val, Byte *data)
	{
		Int valPos = (Int)math::abs(val);

		Int lastIndex = 0;
		if (valPos >= 100000)
		{
			if (valPos >= 10000000)
			{
				if (valPos >= 1000000000) lastIndex = 9;
				else if (valPos >= 100000000) lastIndex = 8;
				else lastIndex = 7;
			}
			else
			{
				if (valPos >= 1000000) lastIndex = 6;
				else lastIndex = 5;
			}
		}
		else
		{
			if (valPos >= 1000)
			{
				if (valPos >= 10000) lastIndex = 4;
				else lastIndex = 3;
			}
			else
			{
				if (valPos >= 100) lastIndex = 2;
				else if (valPos >= 10) lastIndex = 1;
			}
		}

		Int lowerBound = 0;
		if (val < 0)
		{
			data[0] = (Byte)'-';
			lastIndex++;
			lowerBound++;
		}

		for (Int i = lastIndex; i >= lowerBound; i--)
		{
			data[i] = valPos % 10 + 48;
			valPos /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeLong(Long val, Byte *data)
	{
		ULong valPos = (ULong)math::abs(val);

		Int lastIndex = 0;
		if (valPos >= 10000000000ull)
		{
			if (valPos >= 1000000000000000ull)
			{
				if (valPos >= 100000000000000000ull)
				{
					if (valPos >= 10000000000000000000ull) lastIndex = 19;
					else if (valPos >= 1000000000000000000ull) lastIndex = 18;
					else lastIndex = 17;
				}
				else
				{
					if (valPos >= 10000000000000000ull) lastIndex = 16;
					else lastIndex = 15;
				}
			}
			else
			{
				if (valPos >= 10000000000000ull)
				{
					if (valPos >= 100000000000000ull) lastIndex = 14;
					else lastIndex = 13;
				}
				else
				{
					if (valPos >= 1000000000000ull) lastIndex = 12;
					else if (valPos >= 100000000000ull) lastIndex = 11;
					else lastIndex = 10;
				}
			}
		}
		else
		{
			if (valPos >= 100000ull)
			{
				if (valPos >= 10000000ull)
				{
					if (valPos >= 1000000000ull) lastIndex = 9;
					else if (valPos >= 100000000ull) lastIndex = 8;
					else lastIndex = 7;
				}
				else
				{
					if (valPos >= 1000000ull) lastIndex = 6;
					else lastIndex = 5;
				}
			}
			else
			{
				if (valPos >= 1000ull)
				{
					if (valPos >= 10000ull) lastIndex = 4;
					else lastIndex = 3;
				}
				else
				{
					if (valPos >= 100ull) lastIndex = 2;
					else if (valPos >= 10ull) lastIndex = 1;
				}
			}
		}

		Int lowerBound = 0;
		if (val < 0)
		{
			data[0] = (Byte)'-';
			lastIndex++;
			lowerBound++;
		}

		for (Int i = lastIndex; i >= lowerBound; i--)
		{
			data[i] = valPos % 10 + 48;
			valPos /= 10;
		}
		return lastIndex + 1;
	}

	inline UInt writeFloat(Float val, Float valPos, Byte *data)
	{
		if (valPos >= 1000000.0f)
		{
			return printToString((Char*)data, 32, "%.6e", val);
		}

		UInt intValPos = (UInt)valPos;
		Float floatValPos = valPos - intValPos;
		UInt intDigits = 0;
		UInt curIndex = writeIntPart((Char*)data, val < 0, intValPos, intDigits);
		Int floatDigits = intValPos == 0 ? 11 : (6 - intDigits);
		if (floatDigits == 0 || floatValPos == 0)
		{
			return curIndex;
		}

		initTenPowers();

		data[curIndex++] = '.';

		ULong floatValPosClip = (ULong)math::round(floatValPos * tenPowers[floatDigits]);
		if (floatValPosClip >= tenPowers[floatDigits]) floatValPosClip = tenPowers[floatDigits] - 1;

		Int clipDigits = 1;
		if (floatValPosClip >= 10000ull)
		{
			if (floatValPosClip >= 100000000ull)
			{
				if (floatValPosClip >= 10000000000ull) clipDigits = 11;
				else if (floatValPosClip >= 1000000000ull) clipDigits = 10;
				else clipDigits = 9;
			}
			else
			{
				if (floatValPosClip >= 1000000ull)
				{
					if (floatValPosClip >= 10000000ull) clipDigits = 8;
					else clipDigits = 7;
				}
				else
				{
					if (floatValPosClip >= 100000ull) clipDigits = 6;
					else clipDigits = 5;
				}
			}
		}
		else
		{
			if (floatValPosClip >= 100ull)
			{
				if (floatValPosClip >= 1000ull) clipDigits = 4;
				else clipDigits = 3;
			}
			else
			{
				if (floatValPosClip >= 10ull) clipDigits = 2;
			}
		}

		Int discardDigits = math::max(0, clipDigits - 6);
		if (discardDigits > 0)
		{
			floatValPosClip /= tenPowers[discardDigits];
			floatDigits = math::max(1, floatDigits - discardDigits);
		}

		for (Int i = floatDigits - 1; i >= 0; i--)
		{
			data[curIndex + i] = floatValPosClip % 10 + 48;
			floatValPosClip /= 10;
		}

		floatDigits = trimFloat((Char*)&data[curIndex], floatDigits);
		if (floatDigits > 0) return curIndex + floatDigits;
		else if (floatDigits == 0) return curIndex - 1;
		else return writeIntPart((Char*)data, val < 0, intValPos + 1, intDigits);
	}

	inline UInt writeDouble(Double val, Double valPos, Byte *data)
	{
		if (valPos >= 10000000000.0)
		{
			return printToString((Char*)data, 64, "%le", val);
		}

		ULong intValPos = (ULong)valPos;
		Double floatValPos = valPos - intValPos;
		UInt intDigits = 0;
		UInt curIndex = writeIntPart((Char*)data, val < 0, intValPos, intDigits);
		Int floatDigits = intValPos == 0 ? 18 : (10 - intDigits);
		if (floatDigits == 0 || floatValPos == 0)
		{
			return curIndex;
		}

		initTenPowers();

		data[curIndex++] = '.';

		ULong floatValPosClip = (ULong)math::round(floatValPos * tenPowers[floatDigits]);
		if (floatValPosClip >= tenPowers[floatDigits]) floatValPosClip = tenPowers[floatDigits] - 1;

		Int clipDigits = 1;
		if (floatValPosClip >= 10000000000ull)
		{
			if (floatValPosClip >= 1000000000000000ull)
			{
				if (floatValPosClip >= 100000000000000000ull)
				{
					if (floatValPosClip >= 1000000000000000000ull) clipDigits = 19;
					else clipDigits = 18;
				}
				else
				{
					if (floatValPosClip >= 10000000000000000ull) clipDigits = 17;
					else clipDigits = 16;
				}
			}
			else
			{
				if (floatValPosClip >= 10000000000000ull)
				{
					if (floatValPosClip >= 100000000000000ull) clipDigits = 15;
					else clipDigits = 14;
				}
				else
				{
					if (floatValPosClip >= 1000000000000ull) clipDigits = 13;
					else if (floatValPosClip >= 100000000000ull) clipDigits = 12;
					else clipDigits = 11;
				}
			}
		}
		else
		{
			if (floatValPosClip >= 100000)
			{
				if (floatValPosClip >= 10000000)
				{
					if (floatValPosClip >= 1000000000) clipDigits = 10;
					else if (floatValPosClip >= 100000000) clipDigits = 9;
					else clipDigits = 8;
				}
				else
				{
					if (floatValPosClip >= 1000000) clipDigits = 7;
					else clipDigits = 6;
				}
			}
			else
			{
				if (floatValPosClip >= 1000)
				{
					if (floatValPosClip >= 10000) clipDigits = 5;
					else clipDigits = 4;
				}
				else
				{
					if (floatValPosClip >= 100) clipDigits = 3;
					else if (floatValPosClip >= 10) clipDigits = 2;
				}
			}
		}

		Int discardDigits = math::max(0, clipDigits - 10);
		if (discardDigits > 0)
		{
			floatValPosClip /= tenPowers[discardDigits];
			floatDigits = math::max(1, floatDigits - discardDigits);
		}

		for (Int i = floatDigits - 1; i >= 0; i--)
		{
			data[curIndex + i] = floatValPosClip % 10 + 48;
			floatValPosClip /= 10;
		}

		floatDigits = trimFloat((Char*)&data[curIndex], floatDigits);
		if (floatDigits > 0) return curIndex + floatDigits;
		else if (floatDigits == 0) return curIndex - 1;
		else return writeIntPart((Char*)data, val < 0, intValPos + 1, intDigits);
	}
}

using namespace spadas;
using namespace string_internal;

String::String()
{}

void String::initBuffer(UInt dataSize)
{
	Byte* newVarsRaw = new Byte[sizeof(StringVars) + dataSize];
	StringVars* newVars = new (newVarsRaw)StringVars(dataSize, &newVarsRaw[sizeof(StringVars)]);
	setVars(newVars, TRUE);
}

String::String(Char character)
{
	if (character == 0) return;

	initBuffer(4);
	vars->data[0] = (Byte)character;
	vars->length = 1;
}

String::String(WChar character)
{
	if ((UInt)character < 256)
	{
		if (character == 0) return;

		initBuffer(4);
		vars->data[0] = (Byte)character;
		vars->length = 1;
	}
	else
	{
		initBuffer(8);
		vars->length = wCharToUTF8(&character, 1, (Char*)vars->data, vars->size);
	}
}

String::String(Char text[])
{
	if (text == 0) return;

	UInt textLength = lengthChar(text);
	if (textLength <= 1)
	{
		if (textLength == 0) return;

		initBuffer(4);
		vars->data[0] = (Byte)text[0];
		vars->length = 1;
	}
	else
	{
		initBuffer(textLength);
		utility::memoryCopy(text, vars->data, textLength);
		vars->length = textLength;
	}
}

String::String(const Char text[])
{
	if (text == 0) return;

	UInt textLength = lengthChar(text);
	if (textLength <= 1)
	{
		if (textLength == 0) return;

		initBuffer(4);
		vars->data[0] = (Byte)text[0];
		vars->length = 1;
	}
	else
	{
		initBuffer(textLength);
		utility::memoryCopy((Pointer)&text[0], vars->data, textLength);
		vars->length = textLength;
	}
}

String::String(WChar text[])
{
	if (text == 0) return;

	UInt textLength = lengthWChar(text);
	if (textLength == 0) return;

	if (textLength == 1 && (UInt)text[0] < 256)
	{
		initBuffer(4);
		vars->data[0] = (Byte)text[0];
		vars->length = 1;
	}
	else
	{
		initBuffer(textLength * 4 + 4);
		vars->length = wCharToUTF8(text, textLength, (Char*)vars->data, vars->size);
	}
}

String::String(const WChar text[])
{
	if (text == 0) return;

	UInt textLength = lengthWChar(text);
	if (textLength == 0) return;

	if (textLength == 1 && (UInt)text[0] < 256)
	{
		initBuffer(4);
		vars->data[0] = (Byte)text[0];
		vars->length = 1;
	}
	else
	{
		initBuffer(textLength * 4 + 4);
		vars->length = wCharToUTF8(text, textLength, (Char*)vars->data, vars->size);
	}
}

String::String(Array<Char> text)
{
	if (text.isEmpty()) return;

	UInt textLength = 0;
	Char *textData = text.data();
	for (; textLength < text.size(); textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	initBuffer(textLength);
	utility::memoryCopy(text.data(), vars->data, textLength);
	vars->length = textLength;
}

String::String(Array<WChar> text)
{
	if (text.isEmpty()) return;

	UInt textLength = 0;
	WChar *textData = text.data();
	for (; textLength < text.size(); textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	initBuffer(textLength * 4 + 4);
	vars->length = wCharToUTF8(textData, textLength, (Char*)vars->data, vars->size);
}

String::String(Bool val)
{
	initBuffer(8);
	vars->length = writeBool(val, vars->data);
}

String::String(Byte val)
{
	initBuffer(4);
	vars->length = writeByte(val, vars->data);
}

String::String(Word val)
{
	initBuffer(8);
	vars->length = writeWord(val, vars->data);
}

String::String(UInt val)
{
	initBuffer(16);
	vars->length = writeUInt(val, vars->data);
}

String::String(UInt val, UInt nDigits)
{
	nDigits = math::clamp(nDigits, 1u, 10u);

	UInt validDigits = 1;
	if (val >= 100000)
	{
		if (val >= 10000000)
		{
			if (val >= 1000000000) validDigits = 10;
			else if (val >= 100000000) validDigits = 9;
			else validDigits = 8;
		}
		else
		{
			if (val >= 1000000) validDigits = 7;
			else validDigits = 6;
		}
	}
	else
	{
		if (val >= 1000)
		{
			if (val >= 10000) validDigits = 5;
			else validDigits = 4;
		}
		else
		{
			if (val >= 100) validDigits = 3;
			else if (val >= 10) validDigits = 2;
		}
	}
	SPADAS_ERROR_RETURN(validDigits > nDigits);

	initBuffer(nDigits);
	for (UInt i = 0; i < nDigits - validDigits; i++)
	{
		vars->data[i] = 48;
	}
	for (UInt i = 0; i < validDigits; i++)
	{
		vars->data[nDigits - 1 - i] = val % 10 + 48;
		val /= 10;
	}
	vars->length = nDigits;
}

String::String(ULong val)
{
	initBuffer(32);
	vars->length = writeULong(val, vars->data);
}

String::String(ULong val, UInt nDigits)
{
	nDigits = math::clamp(nDigits, 1u, 20u);

	UInt validDigits = 1;
	if (val >= 10000000000ull)
	{
		if (val >= 1000000000000000ull)
		{
			if (val >= 100000000000000000ull)
			{
				if (val >= 10000000000000000000ull) validDigits = 20;
				else if (val >= 1000000000000000000ull) validDigits = 19;
				else validDigits = 18;
			}
			else
			{
				if (val >= 10000000000000000ull) validDigits = 17;
				else validDigits = 16;
			}
		}
		else
		{
			if (val >= 10000000000000ull)
			{
				if (val >= 100000000000000ull) validDigits = 15;
				else validDigits = 14;
			}
			else
			{
				if (val >= 1000000000000ull) validDigits = 13;
				else if (val >= 100000000000ull) validDigits = 12;
				else validDigits = 11;
			}
		}
	}
	else
	{
		if (val >= 100000ull)
		{
			if (val >= 10000000ull)
			{
				if (val >= 1000000000ull) validDigits = 10;
				else if (val >= 100000000ull) validDigits = 9;
				else validDigits = 8;
			}
			else
			{
				if (val >= 1000000ull) validDigits = 7;
				else validDigits = 6;
			}
		}
		else
		{
			if (val >= 1000ull)
			{
				if (val >= 10000ull) validDigits = 5;
				else validDigits = 4;
			}
			else
			{
				if (val >= 100ull) validDigits = 3;
				else if (val >= 10ull) validDigits = 2;
			}
		}
	}
	SPADAS_ERROR_RETURN(validDigits > nDigits);

	initBuffer(nDigits);
	for (UInt i = 0; i < nDigits - validDigits; i++)
	{
		vars->data[i] = 48;
	}
	for (UInt i = 0; i < validDigits; i++)
	{
		vars->data[nDigits - 1 - i] = val % 10 + 48;
		val /= 10;
	}
	vars->length = nDigits;
}

String::String(Short val)
{
	initBuffer(8);
	vars->length = writeShort(val, vars->data);
}

String::String(Int val)
{
	initBuffer(16);
	vars->length = writeInt(val, vars->data);
}

String::String(Long val)
{
	initBuffer(32);
	vars->length = writeLong(val, vars->data);
}

String::String(Float val)
{
	Float valPos = math::abs(val);
	if (valPos <= 0.000001f)
	{
		initBuffer(4);
		vars->data[0] = (Byte)'0';
		vars->length = 1;
	}
	else
	{
		initBuffer(32);
		vars->length = writeFloat(val, valPos, vars->data);
	}
}

String::String(Double val)
{
	Double valPos = math::abs(val);
	if (valPos <= 0.0000000001)
	{
		initBuffer(4);
		vars->data[0] = (Byte)'0';
		vars->length = 1;
	}
	else
	{
		initBuffer(64);
		vars->length = writeDouble(val, valPos, vars->data);
	}
}

String::String(Float val, UInt nDigits)
{
	nDigits = math::clamp(nDigits, 1u, 9u);

	Float valPos = math::abs(val);
	if (valPos >= 1000000.0f)
	{
		initBuffer(64);
		Char format[16];
		printToString(format, 16, "%%.%df", nDigits);
		vars->length = printToString((Char*)vars->data, 64, format, val);
		return;
	}

	initBuffer(32);

	UInt intValPos = (UInt)valPos;
	Float floatValPos = valPos - intValPos;
	UInt intDigits = 1;
	if (intValPos >= 100)
	{
		if (intValPos >= 10000)
		{
			if (intValPos >= 100000) intDigits = 6;
			else intDigits = 5;
		}
		else
		{
			if (intValPos >= 1000) intDigits = 4;
			else intDigits = 3;
		}
	}
	else
	{
		if (intValPos >= 10) intDigits = 2;
	}

	UInt curIndex = 0;
	if (val < 0) vars->data[curIndex++] = '-';
	for (Int i = intDigits - 1; i >= 0; i--)
	{
		vars->data[curIndex + i] = intValPos % 10 + 48;
		intValPos /= 10;
	}
	curIndex += intDigits;

	if (nDigits == 0)
	{
		vars->length = curIndex;
		return;
	}

	initTenPowers();

	vars->data[curIndex++] = '.';

	ULong floatValPosClip = (ULong)math::round(floatValPos * tenPowers[nDigits]);
	if (floatValPosClip >= tenPowers[nDigits]) floatValPosClip = tenPowers[nDigits] - 1;

	for (Int i = (Int)nDigits - 1; i >= 0; i--)
	{
		vars->data[curIndex + i] = floatValPosClip % 10 + 48;
		floatValPosClip /= 10;
	}
	curIndex += nDigits;

	vars->length = curIndex;
}

String::String(Double val, UInt nDigits)
{
	nDigits = math::clamp(nDigits, 1u, 18u);

	Double valPos = math::abs(val);
	if (valPos >= 1000000000000000.0)
	{
		initBuffer(384);
		Char format[16];
		printToString(format, 16, "%%.%dlf", nDigits);
		vars->length = printToString((Char*)vars->data, 384, format, val);
		return;
	}

	initBuffer(64);

	ULong intValPos = (ULong)valPos;
	Double floatValPos = valPos - intValPos;
	UInt intDigits = 1;
	if (intValPos >= 10000ull)
	{
		if (intValPos >= 100000000ull)
		{
			if (intValPos >= 1000000000000ull)
			{
				if (intValPos >= 100000000000000ull) intDigits = 15;
				else if (intValPos >= 10000000000000ull) intDigits = 14;
				else intDigits = 13;
			}
			else
			{
				if (intValPos >= 10000000000ull)
				{
					if (intValPos >= 100000000000ull) intDigits = 12;
					else intDigits = 11;
				}
				else
				{
					if (intValPos >= 1000000000ull) intDigits = 10;
					else intDigits = 9;
				}
			}
		}
		else
		{
			if (intValPos >= 1000000ull)
			{
				if (intValPos >= 10000000ull) intDigits = 8;
				else intDigits = 7;
			}
			else
			{
				if (intValPos >= 100000ull) intDigits = 6;
				else intDigits = 5;
			}
		}
	}
	else
	{
		if (intValPos >= 100ull)
		{
			if (intValPos >= 1000ull) intDigits = 4;
			else intDigits = 3;
		}
		else
		{
			if (intValPos >= 10ull) intDigits = 2;
		}
	}

	UInt curIndex = 0;
	if (val < 0) vars->data[curIndex++] = '-';
	for (Int i = intDigits - 1; i >= 0; i--)
	{
		vars->data[curIndex + i] = intValPos % 10 + 48;
		intValPos /= 10;
	}
	curIndex += intDigits;

	if (nDigits == 0)
	{
		vars->length = curIndex;
		return;
	}

	initTenPowers();

	vars->data[curIndex++] = '.';

	ULong floatValPosClip = (ULong)math::round(floatValPos * tenPowers[nDigits]);
	if (floatValPosClip >= tenPowers[nDigits]) floatValPosClip = tenPowers[nDigits] - 1;

	for (Int i = (Int)nDigits - 1; i >= 0; i--)
	{
		vars->data[curIndex + i] = floatValPosClip % 10 + 48;
		floatValPosClip /= 10;
	}
	curIndex += nDigits;

	vars->length = curIndex;
}

String::String(Binary binary)
{
    if (binary.isEmpty()) return;

	UInt textLength = 0;
	Byte *textData = binary.data();
	UInt binarySize = binary.size();
	for (; textLength < binarySize; textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	initBuffer(textLength);
	utility::memoryCopy(textData, vars->data, textLength);
	vars->length = textLength;
}

String::String(BinarySpan span)
{
    if (span.isEmpty()) return;

	UInt textLength = 0;
	Byte *textData = span.data();
	UInt binarySize = span.size();
	for (; textLength < binarySize; textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	initBuffer(textLength);
	utility::memoryCopy(textData, vars->data, textLength);
	vars->length = textLength;
}

Bool String::operator ==(String string)
{
	UInt thisLength = length();
	UInt targetLength = string.length();
	if (thisLength != targetLength) return FALSE;
	else if (thisLength == 0) return TRUE;
	else return isSameString(vars->data, string.vars->data, thisLength);
}

Bool String::operator !=(String string)
{
	UInt thisLength = length();
	UInt targetLength = string.length();
	if (thisLength != targetLength) return TRUE;
	else if (thisLength == 0) return FALSE;
	else return !isSameString(vars->data, string.vars->data, thisLength);
}

Bool String::operator >(String string)
{
	if (length() == 0)
	{
		return FALSE;
	}
	else
	{
		if (string.length() == 0) return TRUE;
		else return compareString(vars->data, vars->length, string.vars->data, string.vars->length) > 0;
	}
}

Bool String::operator <(String string)
{
	if (length() == 0)
	{
		return string.length() != 0;
	}
	else
	{
		if (string.length() == 0) return FALSE;
		else return compareString(vars->data, vars->length, string.vars->data, string.vars->length) < 0;
	}
}

const Byte *String::bytes()
{
	return vars ? vars->data : NULL;
}

UInt String::length()
{
	return vars ? vars->length : 0;
}

Bool String::isEmpty()
{
	return !vars || vars->length == 0;
}

Word String::getHash()
{
	if (vars) return StringCommon::getHashCode(vars->data, vars->length);
	else return 0;
}

String String::clone()
{
	if (vars) return StringCommon::clone(vars->data, vars->length);
	else return String();
}

Array<Char> String::chars()
{
	return StringCommon::chars(bytes(), length());
}

Array<WChar> String::wchars()
{
	return StringCommon::wchars(bytes(), length());
}

StringAppender String::operator +(String string)
{
	return StringCommon::operatorPlus(bytes(), length(), string);
}

StringAppender String::operator +(StringSpan span)
{
	return StringCommon::operatorPlusSpan(bytes(), length(), span);
}

Optional<Int> String::toInt()
{
	if (vars) return StringCommon::toInt(vars->data, vars->length);
	else return Optional<Int>();
}

Int String::toInt(Int defaultValue)
{
	if (vars) return StringCommon::toInt(vars->data, vars->length, defaultValue);
	else return defaultValue;
}

Optional<Long> String::toLong()
{
	if (vars) return StringCommon::toLong(vars->data, vars->length);
	else return Optional<Long>();
}

Long String::toLong(Long defaultValue)
{
	if (vars) return StringCommon::toLong(vars->data, vars->length, defaultValue);
	else return defaultValue;
}

Optional<Float> String::toFloat()
{
	if (vars) return StringCommon::toFloat(vars->data, vars->length);
	else return Optional<Float>();
}

Float String::toFloat(Float defaultValue)
{
	if (vars) return StringCommon::toFloat(vars->data, vars->length, defaultValue);
	else return defaultValue;
}

Optional<Double> String::toDouble()
{
	if (vars) return StringCommon::toDouble(vars->data, vars->length);
	else return Optional<Double>();
}

Double String::toDouble(Double defaultValue)
{
	if (vars) return StringCommon::toDouble(vars->data, vars->length, defaultValue);
	else return defaultValue;
}

Bool String::toNumber(Int& number)
{
	if (vars) return StringCommon::toNumber(vars->data, vars->length, number);
	else return FALSE;
}

Bool String::toNumber(Long& number)
{
	if (vars) return StringCommon::toNumber(vars->data, vars->length, number);
	else return FALSE;
}

Bool String::toNumber(Float& number)
{
	if (vars) return StringCommon::toNumber(vars->data, vars->length, number);
	else return FALSE;
}

Bool String::toNumber(Double& number)
{
	if (vars) return StringCommon::toNumber(vars->data, vars->length, number);
	else return FALSE;
}

Binary String::toBinary()
{
	if (vars) return StringCommon::toBinary(vars->data, vars->length);
	else return Binary();
}

String String::toUpper()
{
	if (vars) return StringCommon::toUpper(vars->data, vars->length);
	else return String();
}

String String::toLower()
{
	if (vars) return StringCommon::toLower(vars->data, vars->length);
	else return String();
}

Bool String::startsWith(String target)
{
	if (vars) return StringCommon::startsWith(vars->data, vars->length, target);
	else return FALSE;
}

Bool String::endsWith(String target)
{
	if (vars) return StringCommon::endsWith(vars->data, vars->length, target);
	else return FALSE;
}

Array<UInt> String::search(String target)
{
	if (vars) return StringCommon::search(vars->data, vars->length, target);
	else return Array<UInt>();
}

UInt String::searchFirst(String target)
{
	if (vars) return StringCommon::searchFirst(vars->data, vars->length, target);
	else return UINF;
}

UInt String::searchLast(String target)
{
	if (vars) return StringCommon::searchLast(vars->data, vars->length, target);
	else return UINF;
}

String String::trim()
{
	if (vars) return StringCommon::trim(vars->data, vars->length);
	else return String();
}

Array<StringSpan> String::split(String target)
{
	if (vars) return StringCommon::split(vars->data, vars->length, target, vars);
	else return Array<StringSpan>();
}

Array<String> String::splitToStringArray(String target)
{
	return split(target).convert<String>([](StringSpan& s){ return s.clone(); });
}

String String::replace(String oldString, String newString)
{
	return StringCommon::replace(bytes(), length(), oldString, newString);
}

StringSpan String::span(UInt index, UInt length)
{
	return StringSpan(*this, index, length);
}

void String::ensureBuffer(UInt appendSize)
{
	if (!vars)
	{
		initBuffer(appendSize);
		return;
	}

	UInt originLength = vars->length;
	UInt totalLength = originLength + appendSize;
	if (totalLength > vars->size)
	{
		Byte *dstData = new Byte[totalLength * 2];
		utility::memoryCopy(vars->data, dstData, originLength);
		if (vars->extData) delete[] vars->extData;
		vars->data = vars->extData = dstData;
		vars->size = totalLength * 2;
	}
}

void String::operator +=(String append)
{
	UInt appendLength = append.length();
	if (appendLength == 0) return;

	UInt originLength = length();
	
	ensureBuffer(appendLength);
	utility::memoryCopy(append.vars->data, &vars->data[originLength], appendLength);
	vars->length = originLength + appendLength;
}

void String::operator +=(StringSpan span)
{
	UInt appendLength = span.length();
	if (appendLength == 0) return;

	UInt originLength = length();
	
	ensureBuffer(appendLength);
	utility::memoryCopy(span.bytes(), &vars->data[originLength], appendLength);
	vars->length = originLength + appendLength;
}

void String::operator +=(Char character)
{
	if (character == 0) return;

	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(4);
	vars->data[originLength] = (Byte)character;
	vars->length = originLength + 1;
}

void String::operator +=(WChar character)
{
	if ((UInt)character < 256)
	{
		if (character == 0) return;

		UInt originLength = vars ? vars->length : 0;
		ensureBuffer(4);
		vars->data[originLength] = (Byte)character;
		vars->length = originLength + 1;
	}
	else
	{
		UInt originLength = vars ? vars->length : 0;
		ensureBuffer(8);
		vars->length = originLength + wCharToUTF8(&character, 1, (Char*)&vars->data[originLength], vars->size - originLength);
	}
}

void String::operator +=(Char text[])
{
	if (text == 0) return;

	UInt textLength = lengthChar(text);
	if (textLength <= 1)
	{
		if (textLength == 0) return;

		UInt originLength = vars ? vars->length : 0;
		ensureBuffer(4);
		vars->data[originLength] = (Byte)text[0];
		vars->length = originLength + 1;
	}
	else
	{
		UInt originLength = vars ? vars->length : 0;
		ensureBuffer(textLength);
		utility::memoryCopy(text, &vars->data[originLength], textLength);
		vars->length = originLength + textLength;
	}
}

void String::operator +=(const Char text[])
{
	if (text == 0) return;

	UInt textLength = lengthChar(text);
	if (textLength <= 1)
	{
		if (textLength == 0) return;

		UInt originLength = vars ? vars->length : 0;
		ensureBuffer(4);
		vars->data[originLength] = (Byte)text[0];
		vars->length = originLength + 1;
	}
	else
	{
		UInt originLength = vars ? vars->length : 0;
		ensureBuffer(textLength);
		utility::memoryCopy((Pointer)&text[0], &vars->data[originLength], textLength);
		vars->length = originLength + textLength;
	}
}

void String::operator +=(WChar text[])
{
	if (text == 0) return;

	UInt textLength = lengthWChar(text);
	if (textLength == 0) return;

	UInt originLength = vars ? vars->length : 0;
	if (textLength == 1 && (UInt)text[0] < 256)
	{
		ensureBuffer(4);
		vars->data[originLength] = (Byte)text[0];
		vars->length = originLength + 1;
	}
	else
	{
		ensureBuffer(textLength * 4 + 4);
		vars->length = originLength + wCharToUTF8(text, textLength, (Char*)&vars->data[originLength], vars->size - originLength);
	}
}

void String::operator +=(const WChar text[])
{
	if (text == 0) return;

	UInt textLength = lengthWChar(text);
	if (textLength == 0) return;

	UInt originLength = vars ? vars->length : 0;
	if (textLength == 1 && (UInt)text[0] < 256)
	{
		ensureBuffer(4);
		vars->data[originLength] = (Byte)text[0];
		vars->length = originLength + 1;
	}
	else
	{
		ensureBuffer(textLength * 4 + 4);
		vars->length = originLength + wCharToUTF8(text, textLength, (Char*)&vars->data[originLength], vars->size - originLength);
	}
}

void String::operator +=(Array<Char> text)
{
	if (text.isEmpty()) return;

	UInt textLength = 0;
	Char *textData = text.data();
	for (; textLength < text.size(); textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(textLength);
	utility::memoryCopy(text.data(), &vars->data[originLength], textLength);
	vars->length = originLength + textLength;
}

void String::operator +=(Array<WChar> text)
{
	if (text.isEmpty()) return;

	UInt textLength = 0;
	WChar *textData = text.data();
	for (; textLength < text.size(); textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(textLength * 4 + 4);
	vars->length = originLength + wCharToUTF8(textData, textLength, (Char*)&vars->data[originLength], vars->size - originLength);
}

void String::operator +=(Bool val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(8);
	vars->length = originLength + writeBool(val, &vars->data[originLength]);
}

void String::operator +=(Byte val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(4);
	vars->length = originLength + writeByte(val, &vars->data[originLength]);
}

void String::operator +=(Word val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(8);
	vars->length = originLength + writeWord(val, &vars->data[originLength]);
}

void String::operator +=(UInt val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(16);
	vars->length = originLength + writeUInt(val, &vars->data[originLength]);
}

void String::operator +=(ULong val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(32);
	vars->length = originLength + writeULong(val, &vars->data[originLength]);
}

void String::operator +=(Short val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(8);
	vars->length = originLength + writeShort(val, &vars->data[originLength]);
}

void String::operator +=(Int val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(16);
	vars->length = originLength + writeInt(val, &vars->data[originLength]);
}

void String::operator +=(Long val)
{
	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(32);
	vars->length = originLength + writeLong(val, &vars->data[originLength]);
}

void String::operator +=(Float val)
{
	Float valPos = math::abs(val);
	UInt originLength = vars ? vars->length : 0;
	if (valPos <= 0.000001f)
	{
		ensureBuffer(4);
		vars->data[originLength] = (Byte)'0';
		vars->length = originLength + 1;
	}
	else
	{
		ensureBuffer(32);
		vars->length = originLength + writeFloat(val, valPos, &vars->data[originLength]);
	}
}

void String::operator +=(Double val)
{
	Double valPos = math::abs(val);
	UInt originLength = vars ? vars->length : 0;
	if (valPos <= 0.0000000001)
	{
		ensureBuffer(4);
		vars->data[originLength] = (Byte)'0';
		vars->length = originLength + 1;
	}
	else
	{
		ensureBuffer(64);
		vars->length = originLength + writeDouble(val, valPos, &vars->data[originLength]);
	}
}

void String::operator +=(Binary binary)
{
    if (binary.isEmpty()) return;

	UInt textLength = 0;
	Byte *textData = binary.data();
	UInt binarySize = binary.size();
	for (; textLength < binarySize; textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(textLength);
	utility::memoryCopy(textData, &vars->data[originLength], textLength);
	vars->length = originLength + textLength;
}

void String::operator +=(BinarySpan span)
{
    if (span.isEmpty()) return;

	UInt textLength = 0;
	Byte *textData = span.data();
	UInt binarySize = span.size();
	for (; textLength < binarySize; textLength++)
	{
		if (textData[textLength] == 0) break;
	}
	if (textLength == 0) return;

	UInt originLength = vars ? vars->length : 0;
	ensureBuffer(textLength);
	utility::memoryCopy(textData, &vars->data[originLength], textLength);
	vars->length = originLength + textLength;
}