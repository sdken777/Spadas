
#include "spadas.h"

#include "string_bz.h"

using namespace spadas;
using namespace spadas::math;
using namespace spadas_internal;

Byte staticZeroByte = 0;

Bool spadas_internal::doubleToPrettyString(Double val, Char output[48], Int& length)
{
	if (val > 1000000000000000000.0 || val < -1000000000000000000.0) return FALSE;

	Long valInt = (Long)val;
	if (val == valInt)
	{
		int len = spadas_internal::printToString(output, 48, "%lld", valInt);
		length = len;
		return TRUE;
	}

	int len = spadas_internal::printToString(output, 48, "%.16f", val);

	if (output[len - 1] == '0')
	{
		len--;
		for (Int i = len - 1; i >= 0; i--)
		{
			if (output[i] != '0') break;
			len--;
		}
		if (output[len - 1] == '.')
		{
			len--;
		}
	}

	if (output[len - 1] == '9')
	{
		UInt nineCount = 1;
		for (Int i = len - 2; i >= 0; i--)
		{
			if (output[i] != '9') break;
			nineCount++;
		}
		if (nineCount > 3) len -= (Int)(nineCount - 3);
	}
	else if (output[len - 1] == '1')
	{
		UInt zeroCount = 0;
		for (Int i = len - 2; i >= 0; i--)
		{
			if (output[i] != '0') break;
			zeroCount++;
		}
		if (zeroCount > 3) len -= (Int)(zeroCount + 1);
		if (output[len - 1] == '.')
		{
			len--;
		}
	}

	if (len < 0) len = 0;
	output[len] = 0;
	length = len;

	return TRUE;
}

const String spadas::String::TypeName = "spadas.String";

String::String()
{}

String::String(Char character)
{
	if (character == 0) return;

	setVars(new StringVars(2), TRUE);
	vars->data[0] = (Byte)character;
	vars->data[1] = 0;
	vars->length = 1;
}

String::String(WChar character)
{
	if (character == 0) return;

	setVars(new StringVars(5), TRUE);
	vars->length = math::min(vars->size - 1, wCharToUTF8(&character, 1, (Char*)vars->data, vars->size));
	vars->data[vars->length] = 0;
}

String::String(const Char text[])
{
	if (text == 0) return;

	UInt textLength = lengthChar(text);
	if (textLength == 0) return;

	setVars(new StringVars(textLength * 2 + 1), TRUE);
	vars->length = math::min(vars->size - 1, charToUTF8(text, textLength, (Char*)vars->data, vars->size));
	vars->data[vars->length] = 0;
}

String::String(const WChar text[])
{
	if (text == 0) return;

	UInt textLength = lengthWChar(text);
	if (textLength == 0) return;

	setVars(new StringVars(textLength * 4 + 1), TRUE);
	vars->length = math::min(vars->size - 1, wCharToUTF8(text, textLength, (Char*)vars->data, vars->size));
	vars->data[vars->length] = 0;
}

String::String(Array<Char> text)
{
	if (text.isEmpty()) return;

	Char* textData = text.data();
	UInt textLength = text.size();

	setVars(new StringVars(textLength * 2 + 1), TRUE);
	vars->length = math::min(vars->size - 1, charToUTF8(textData, textLength, (Char*)vars->data, vars->size));
	vars->data[vars->length] = 0;
}

String::String(Array<WChar> text)
{
	if (text.isEmpty()) return;

	WChar *textData = text.data();
	UInt textLength = text.size();

	setVars(new StringVars(textLength * 4 + 1), TRUE);
	vars->length = math::min(vars->size - 1, wCharToUTF8(textData, textLength, (Char*)vars->data, vars->size));
	vars->data[vars->length] = 0;
}

String::String(Bool val) : Object<StringVars>(new StringVars(6), TRUE)
{
	if (val)
	{
		vars->data[0] = 'T';
		vars->data[1] = 'R';
		vars->data[2] = 'U';
		vars->data[3] = 'E';
		vars->data[4] = 0;
		vars->length = 4;
	}
	else
	{
		vars->data[0] = 'F';
		vars->data[1] = 'A';
		vars->data[2] = 'L';
		vars->data[3] = 'S';
		vars->data[4] = 'E';
		vars->data[5] = 0;
		vars->length = 5;
	}
}

String::String(Byte val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString((Char*)vars->data, 16, "%u", (UInt)val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(Word val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString((Char*)vars->data, 16, "%u", (UInt)val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(UInt val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString((Char*)vars->data, 16, "%u", val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(UInt val, UInt nDigits) : Object<StringVars>(new StringVars(nDigits + 1), TRUE)
{
	UInt *units = new UInt[nDigits];
	for (UInt i = 0; i < nDigits; i++)
	{
		units[i] = val % 10;
		val /= 10;
	}

	for (UInt i = 0; i < nDigits; i++)
	{
		vars->data[i] = (Char)(units[nDigits - i - 1] + 48);
	}
	vars->data[nDigits] = 0;
	vars->length = nDigits;

	delete[] units;
}

String::String(ULong val) : Object<StringVars>(new StringVars(32), TRUE)
{
	UInt length = printToString((Char*)vars->data, 32, "%llu", val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(ULong val, UInt nDigits) : Object<StringVars>(new StringVars(nDigits + 1), TRUE)
{
	UInt *units = new UInt[nDigits];
	for (UInt i = 0; i < nDigits; i++)
	{
		units[i] = (UInt)(val % 10);
		val /= 10;
	}

	for (UInt i = 0; i < nDigits; i++)
	{
		vars->data[i] = (Char)(units[nDigits - i - 1] + 48);
	}
	vars->data[nDigits] = 0;
	vars->length = nDigits;

	delete[] units;
}

String::String(Short val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString((Char*)vars->data, 16, "%d", (Int)val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(Int val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString((Char*)vars->data, 16, "%d", val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(Long val) : Object<StringVars>(new StringVars(32), TRUE)
{
	UInt length = printToString((Char*)vars->data, 32, "%lld", val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(Float val) : Object<StringVars>(new StringVars(32), TRUE)
{
	UInt length = printToString((Char*)vars->data, 32, "%.3f", val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(Double val) : Object<StringVars>(new StringVars(64), TRUE)
{
	UInt length = printToString((Char*)vars->data, 64, "%.3lf", val);
	vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
}

String::String(Float val, UInt nDigits) : Object<StringVars>(new StringVars(48), TRUE)
{
	if (nDigits == UINF)
	{
		Int length = 0;
		if (doubleToPrettyString((Double)val, (Char*)vars->data, length))
		{
			vars->length = length;
		}
		else
		{
			if (val >= 0) val = FINF;
			else val = NFINF;
			printToString((Char*)vars->data, 48, "%f", val);
		}
	}
	else
	{
		Char format[16];
		printToString(format, 16, "%%.%df", math::min(nDigits, 10u));

		UInt length = printToString((Char*)vars->data, 48, format, val);
		vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
	}
}

String::String(Double val, UInt nDigits) : Object<StringVars>(new StringVars(64), TRUE)
{
	if (nDigits == UINF)
	{
		Int length = 0;
		if (doubleToPrettyString(val, (Char*)vars->data, length))
		{
			vars->length = length;
		}
		else
		{
			if (val >= 0) val = DINF;
			else val = NDINF;
			printToString((Char*)vars->data, 64, "%lf", val);
		}
	}
	else
	{
		Char format[16];
		printToString(format, 16, "%%.%dlf", math::min(nDigits, 20u));

		UInt length = printToString((Char*)vars->data, 64, format, val);
		vars->length = length == UINF ? lengthChar((Char*)vars->data) : length;
	}
}

String::String(Binary binary)
{
    if (binary.isEmpty()) return;

	Byte* binData = binary.data();
	UInt binSize = binary.size();
	setVars(new StringVars(binSize + 1), TRUE);

	utility::memoryCopy(binData, vars->data, binSize);
	vars->data[binSize] = 0;
	vars->length = lengthChar((Char*)vars->data);
}

String::String(String src, Region region)
{
	if (src.isEmpty()) return;

	UInt srcLength = src.length();
    Int startIndex = region.offset, endIndex = (region.size == UINF ? INF : (region.offset + (Int)region.size - 1));
	if (region.size == 0 || startIndex >= (Int)srcLength || endIndex < 0) return;
    
    startIndex = math::max(0, startIndex);
    endIndex = math::min((Int)srcLength - 1, endIndex);
    Int length = endIndex + 1 - startIndex;
	if (length <= 0) return;
    
	setVars(new StringVars(length + 1), TRUE);

	utility::memoryCopy(&src.vars->data[startIndex], vars->data, length);
	vars->data[length] = 0;
    vars->length = length;
}

Bool String::operator ==(String string)
{
	if (isEmpty())
	{
		return string.isEmpty();
	}
	else
	{
		if (string.isEmpty()) return FALSE;
		else return compareString((Char*)vars->data, (Char*)string.vars->data) == 0;
	}
}

Bool String::operator !=(String string)
{
	if (isEmpty())
	{
		return !string.isEmpty();
	}
	else
	{
		if (string.isEmpty()) return TRUE;
		else return compareString((Char*)vars->data, (Char*)string.vars->data) != 0;
	}
}

Bool String::operator >(String string)
{
	if (isEmpty())
	{
		return FALSE;
	}
	else
	{
		if (string.isEmpty()) return TRUE;
		else return compareString((Char*)vars->data, (Char*)string.vars->data) > 0;
	}
}

Bool String::operator <(String string)
{
	if (isEmpty())
	{
		return !string.isEmpty();
	}
	else
	{
		if (string.isEmpty()) return FALSE;
		else return compareString((Char*)vars->data, (Char*)string.vars->data) < 0;
	}
}

Word String::getHash()
{
	if (isEmpty()) return 0;
	UInt hash = 0;
	for (UInt i = 0; i < vars->length; i++)
	{
		hash += (UInt)vars->data[i];
		hash *= 3;
	}
	return (Word)(hash & 0x0000ffff);
}

String String::clone()
{
	if (isEmpty()) return String();

	UInt length = vars->length;
	String out = String::createWithSize(length + 1);
	Byte* dstData = out.vars->data;
	utility::memoryCopy(vars->data, dstData, length);
	dstData[length] = 0;
	out.vars->length = length;
	return out;
}

void String::updateLength()
{
	if (!vars) return;
	vars->length = lengthChar((Char*)vars->data);
}

Byte *String::bytes()
{
	if (!vars)
	{
		staticZeroByte = 0;
		return &staticZeroByte;
	}
	else return vars->data;
}

UInt String::byteSize()
{
	return vars ? vars->size : 0;
}

Array<Char> String::chars()
{
	if (isEmpty()) return Array<Char>();

	Array<Char> output(vars->length * 2 + 1);
	UInt outputLength = utf8ToChar((Char*)vars->data, vars->length, output.data(), output.size());
	output[outputLength] = 0;
	output.trim(outputLength + 1);
	return output;
}

Array<WChar> String::wchars()
{
	if (isEmpty()) return Array<WChar>();

	Array<WChar> output(vars->length + 1);
	UInt outputLength = utf8ToWChar((Char*)vars->data, vars->length, output.data(), output.size());
	output[outputLength] = 0;
	output.trim(outputLength + 1);
	return output;
}

UInt String::length()
{
	return vars ? vars->length : 0;
}

Bool String::isEmpty()
{
	return !vars || vars->length == 0/* like createWithSize, updateLength, etc. */;
}

void String::operator +=(String append)
{
	if (append.isEmpty()) return;

	UInt appendLength = append.vars->length;
	if (!vars)
	{
		setVars(new StringVars(appendLength + 1), TRUE);
		utility::memoryCopy(append.vars->data, vars->data, appendLength);
		vars->data[appendLength] = 0;
		vars->length = appendLength;
		return;
	}

	UInt originLength = vars->length;
	UInt totalLength = originLength + appendLength;
	if (totalLength + 1 > vars->size)
	{
		Byte *dstData = new Byte[(totalLength + 1) * 2];
		utility::memoryCopy(vars->data, dstData, originLength);
		delete[] vars->data;
		vars->data = dstData;
		vars->size = (totalLength + 1) * 2;
	}

	utility::memoryCopy(append.vars->data, &vars->data[originLength], appendLength);
	vars->data[totalLength] = 0;
	vars->length = totalLength;
}

String String::operator +(String append)
{
	if (isEmpty()) return append.clone();
	if (append.isEmpty()) return clone();

	UInt originLength = vars->length;
	UInt appendLength = append.vars->length;
	UInt totalLength = originLength + appendLength;

	String out = String::createWithSize(originLength + appendLength + 1);
	utility::memoryCopy(vars->data, out.vars->data, originLength);
	utility::memoryCopy(append.vars->data, &out.vars->data[originLength], appendLength);
	out.vars->data[totalLength] = 0;
	out.vars->length = totalLength;
	return out;
}

Optional<Int> String::toInt()
{
	if (isEmpty()) return Optional<Int>();

	Int val;
	if (scanFromString((Char*)vars->data, vars->size, "%d", &val)) return val;
	else return Optional<Int>();
}

Optional<Long> String::toLong()
{
	if (isEmpty()) return Optional<Long>();

	Long val;
	if (scanFromString((Char*)vars->data, vars->size, "%lld", &val)) return val;
	else return Optional<Long>();
}

Optional<Float> String::toFloat()
{
	if (isEmpty()) return Optional<Float>();

	Float val;
	if (scanFromString((Char*)vars->data, vars->size, "%f", &val)) return val;
	else return Optional<Float>();
}

Optional<Double> String::toDouble()
{
	if (isEmpty()) return Optional<Double>();

	Double val;
	if (scanFromString((Char*)vars->data, vars->size, "%lf", &val)) return val;
	else return Optional<Double>();
}

String String::toUpper()
{
	if (isEmpty()) return String();

	String out = clone();
	Byte *data = out.vars->data;
	UInt length = vars->length;
	for (UInt i = 0; i < length; i++)
	{
		if (data[i] >= 97 && data[i] <= 122) data[i] -= 32;
	}
	return out;
}

String String::toLower()
{
	if (isEmpty()) return String();

	String out = clone();
	Byte *data = out.vars->data;
	UInt length = vars->length;
	for (UInt i = 0; i < length; i++)
	{
		if (data[i] >= 65 && data[i] <= 90) data[i] += 32;
	}
	return out;
}

Bool String::startsWith(String target)
{
	if (isEmpty() || target.isEmpty()) return FALSE;

	UInt srcLength = vars->length;
	UInt targetLength = target.vars->length;
	if (targetLength == 0 || targetLength > srcLength) return FALSE;

	Byte *srcData = vars->data;
	Byte *targetData = target.vars->data;
	for (UInt i = 0; i < targetLength; i++)
	{
		if (srcData[i] != targetData[i]) return FALSE;
	}
	return TRUE;
}

Bool String::endsWith(String target)
{
	if (isEmpty() || target.isEmpty()) return FALSE;

	UInt srcLength = vars->length;
	UInt targetLength = target.vars->length;
	if (targetLength == 0 || targetLength > srcLength) return FALSE;

	Byte *srcData = vars->data;
	Byte *targetData = target.vars->data;
	UInt offset = srcLength - targetLength;
	for (UInt i = 0; i < targetLength; i++)
	{
		if (srcData[offset + i] != targetData[i]) return FALSE;
	}
	return TRUE;
}

Binary String::toBinary()
{
	if (isEmpty()) return Binary();
	else return Binary(vars->data, vars->length);
}

Array<UInt> String::search(String string)
{
	if (isEmpty()) return Array<UInt>();

	SPADAS_ERROR_RETURNVAL(string.isEmpty(), Array<UInt>());

	UInt subLength = string.vars->length;
	Byte *subData = string.vars->data;

	UInt srcLength = vars->length;

	if (subLength > srcLength) return Array<UInt>();

	Array<UInt> bufIndices(srcLength);
	UInt nBufIndices = 0;

	for (UInt i = 0; i <= srcLength - subLength; i++)
	{
		Bool match = TRUE;
		for (UInt n = 0, j = i; n < subLength; n++, j++)
		{
			if (vars->data[j] != subData[n])
			{
				match = FALSE;
				break;
			}
		}

		if (match)
		{
			bufIndices[nBufIndices++] = i;
		}
	}

	bufIndices.trim(nBufIndices);

	return bufIndices;
}

Array<String> String::split(String string)
{
	if (isEmpty()) return Array<String>();
	SPADAS_ERROR_RETURNVAL(string.isEmpty(), Array<String>());

	UInt length = string.vars->length;

	Array<UInt> rawMatches = search(string);
	
	UInt nRawMatches = rawMatches.size();
	if (nRawMatches == 0) return Array<String>::scalar(clone());
	
	Array<UInt> matches(nRawMatches);
	UInt nMatches = 0;
	matches[nMatches++] = rawMatches[0];
	for (UInt i = 1; i < nRawMatches; i++)
	{
		if (rawMatches[i] < matches[nMatches - 1] + length) continue;
		matches[nMatches++] = rawMatches[i];
	}
	matches.trim(nMatches);
	
	UInt outSize = nMatches + 1;
	Array<String> out = Array<String>::createUninitialized(outSize);
	
	UInt index = 0;
	out.initialize(index++, String(*this, Region(0, matches[0])));
	for (UInt i = 0; i < nMatches - 1; i++)
	{
		out.initialize(index++, String(*this, Region(matches[i] + length, math::max((Int)matches[i+1] - (Int)matches[i] - (Int)length, 0))));
	}
	out.initialize(index++, String(*this, Region(matches[nMatches-1] + length, UINF)));
	
	return out;
}

String String::replace(String oldString, String newString)
{
	if (isEmpty()) return String();
	SPADAS_ERROR_RETURNVAL(oldString.isEmpty(), *this);

	UInt srcStringLength = vars->length;
	UInt oldStringLength = oldString.vars->length;
	UInt newStringLength = newString.length(); // vars可能为空
	
	Array<UInt> rawMatches = search(oldString);
	
	UInt nRawMatches = rawMatches.size();
	if (nRawMatches == 0) return clone();
	
	Array<UInt> matches(nRawMatches);
	UInt nMatches = 0;
	matches[nMatches++] = rawMatches[0];
	for (UInt i = 1; i < nRawMatches; i++)
	{
		if (rawMatches[i] < matches[nMatches-1] + oldStringLength) continue;
		matches[nMatches++] = rawMatches[i];
	}
	matches.trim(nMatches);
	
	String out = String::createWithSize(srcStringLength + nMatches * newStringLength - nMatches * oldStringLength + 1);
	out.vars->length = srcStringLength + nMatches * newStringLength - nMatches * oldStringLength;

	Byte *srcStringData = vars->data;
	Byte *newStringData = newString.bytes(); // vars可能为空
	Byte *outData = out.vars->data;

	UInt outIndex = 0;
	if (matches[0] != 0)
	{
		utility::memoryCopy(&srcStringData[0], &outData[outIndex], matches[0]);
		outIndex += matches[0];
	}
	
	for (UInt i = 0; i < nMatches - 1; i++)
	{
		if (newStringLength != 0)
		{
			utility::memoryCopy(newStringData, &outData[outIndex], newStringLength);
			outIndex += newStringLength;
		}
		
		UInt copySize = math::max((Int)matches[i+1] - (Int)matches[i] - (Int)oldStringLength, 0);
		if (copySize != 0)
		{
			utility::memoryCopy(&srcStringData[matches[i] + oldStringLength], &outData[outIndex], copySize);
			outIndex += copySize;
		}
	}
	
	if (newStringLength != 0)
	{
		utility::memoryCopy(newStringData, &outData[outIndex], newStringLength);
		outIndex += newStringLength;
	}
	
	UInt copySize = math::max((Int)srcStringLength - (Int)matches[nMatches-1] - (Int)oldStringLength, 0);
	if (copySize != 0)
	{
		utility::memoryCopy(&srcStringData[matches[nMatches-1] + oldStringLength], &outData[outIndex], copySize);
		outIndex += copySize;
	}
	
	outData[outIndex] = 0;
	return out;
}
