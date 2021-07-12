
#include "spadas.h"

#include "string_bz.h"

using namespace spadas;
using namespace spadas::math;
using namespace spadas_internal;

Char staticZeroChar = 0;
WChar staticZeroWChar = 0;

Bool spadas_internal::doubleToPrettyString(Double val, WChar output[48], Int& length)
{
	if (val > 1000000000000000000.0 || val < -1000000000000000000.0) return FALSE;

	Long valInt = (Long)val;
	if (val == valInt)
	{
		int len = spadas_internal::printToString(output, 48, L"%lld", valInt);
		length = len;
		return TRUE;
	}

	int len = spadas_internal::printToString(output, 48, L"%.16f", val);

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
	vars->data[0] = (WChar)character;
	vars->data[1] = 0;
	vars->length = 1;
}

String::String(WChar character)
{
	if (character == 0) return;

	setVars(new StringVars(2), TRUE);
	vars->data[0] = character;
	vars->data[1] = 0;
	vars->length = 1;
}

String::String(const Char text[])
{
	if (text == 0) return;

	UInt textLength = lengthChar(text);
	if (textLength == 0) return;

	setVars(new StringVars(textLength + 1), TRUE);
	UInt dstLength = charToWChar(text, textLength, vars->data, textLength + 1);

	if (dstLength <= textLength)
	{
		vars->data[dstLength] = 0;
		vars->length = dstLength;
	}
	else
	{
		vars->data[textLength] = 0;
		vars->length = lengthWChar(vars->data);
	}
}

String::String(const WChar text[])
{
	if (text == 0) return;

	UInt textLength = lengthWChar(text);
	if (textLength == 0) return;

	setVars(new StringVars(textLength + 1), TRUE);
	for (UInt i = 0; i < textLength; i++) vars->data[i] = text[i];
	vars->data[textLength] = 0;
	vars->length = textLength;
}

String::String(Array<WChar> text)
{
	if (text.isEmpty()) return;

	WChar *textData = text.data();
	UInt textLength = text.size();

	setVars(new StringVars(textLength + 1), TRUE);
	for (UInt i = 0; i < textLength; i++)
	{
		vars->data[i] = textData[i];
		if (textData[i] == 0)
		{
			vars->length = i;
			return;
		}
	}
	vars->data[textLength] = 0;
	vars->length = textLength;
}

String::String(Bool val) : Object<StringVars>(new StringVars(6), TRUE)
{
	if (val)
	{
		vars->data[0] = L'T';
		vars->data[1] = L'R';
		vars->data[2] = L'U';
		vars->data[3] = L'E';
		vars->data[4] = 0;
		vars->length = 4;
	}
	else
	{
		vars->data[0] = L'F';
		vars->data[1] = L'A';
		vars->data[2] = L'L';
		vars->data[3] = L'S';
		vars->data[4] = L'E';
		vars->data[5] = 0;
		vars->length = 5;
	}
}

String::String(Byte val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString(vars->data, 16, L"%u", (UInt)val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(Word val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString(vars->data, 16, L"%u", (UInt)val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(UInt val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString(vars->data, 16, L"%u", val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
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
		vars->data[i] = (WChar)(units[nDigits - i - 1] + 48);
	}
	vars->data[nDigits] = 0;
	vars->length = nDigits;

	delete[] units;
}

String::String(ULong val) : Object<StringVars>(new StringVars(32), TRUE)
{
	UInt length = printToString(vars->data, 32, L"%llu", val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
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
		vars->data[i] = (WChar)(units[nDigits - i - 1] + 48);
	}
	vars->data[nDigits] = 0;
	vars->length = nDigits;

	delete[] units;
}

String::String(Short val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString(vars->data, 16, L"%d", (Int)val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(Int val) : Object<StringVars>(new StringVars(16), TRUE)
{
	UInt length = printToString(vars->data, 16, L"%d", val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(Long val) : Object<StringVars>(new StringVars(32), TRUE)
{
	UInt length = printToString(vars->data, 32, L"%lld", val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(Float val) : Object<StringVars>(new StringVars(32), TRUE)
{
	UInt length = printToString(vars->data, 32, L"%.3f", val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(Double val) : Object<StringVars>(new StringVars(64), TRUE)
{
	UInt length = printToString(vars->data, 64, L"%.3lf", val);
	vars->length = length == UINF ? lengthWChar(vars->data) : length;
}

String::String(Float val, UInt nDigits) : Object<StringVars>(new StringVars(48), TRUE)
{
	if (nDigits == UINF)
	{
		Int length = 0;
		if (doubleToPrettyString((Double)val, vars->data, length))
		{
			vars->length = length;
		}
		else
		{
			if (val >= 0) val = FINF;
			else val = NFINF;
			printToString(vars->data, 48, L"%f", val);
		}
	}
	else
	{
		WChar format[16];
		printToString(format, 16, L"%%.%df", math::min(nDigits, 10u));

		UInt length = printToString(vars->data, 48, format, val);
		vars->length = length == UINF ? lengthWChar(vars->data) : length;
	}
}

String::String(Double val, UInt nDigits) : Object<StringVars>(new StringVars(64), TRUE)
{
	if (nDigits == UINF)
	{
		Int length = 0;
		if (doubleToPrettyString(val, vars->data, length))
		{
			vars->length = length;
		}
		else
		{
			if (val >= 0) val = DINF;
			else val = NDINF;
			printToString(vars->data, 64, L"%lf", val);
		}
	}
	else
	{
		WChar format[16];
		printToString(format, 16, L"%%.%dlf", math::min(nDigits, 20u));

		UInt length = printToString(vars->data, 64, format, val);
		vars->length = length == UINF ? lengthWChar(vars->data) : length;
	}
}

String::String(Binary binary)
{
    if (binary.isEmpty()) return;

	UInt binSize = binary.size();
	setVars(new StringVars(binSize + 1), TRUE);
	UInt dstLength = utf8ToWChar((Char*)binary.data(), binSize, vars->data, binSize + 1);

	if (dstLength <= binSize)
	{
		vars->data[dstLength] = 0;
		vars->length = dstLength;
	}
	else
	{
		vars->data[binSize] = 0;
		vars->length = lengthWChar(vars->data);
	}
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

	WChar *srcData = src.vars->data;
	WChar *newData = vars->data;
    for (UInt iSrc = startIndex, iDst = 0; iSrc <= (UInt)endIndex; iSrc++, iDst++) newData[iDst] = srcData[iSrc];
    newData[length] = 0;
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
		else return compareString(vars->data, string.vars->data) == 0;
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
		else return compareString(vars->data, string.vars->data) != 0;
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
		else return compareString(vars->data, string.vars->data) > 0;
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
		else return compareString(vars->data, string.vars->data) < 0;
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
	WChar *srcData = vars->data;
	WChar *dstData = out.vars->data;
	for (UInt i = 0; i < length; i++) dstData[i] = srcData[i];
	dstData[length] = 0;
	out.vars->length = length;
	return out;
}

void String::updateLength()
{
	if (!vars) return;
	vars->length = lengthWChar(vars->data);
}

WChar& String::operator [](UInt index)
{
	SPADAS_ERROR_RETURNVAL(!vars || index >= vars->size, *(new WChar));
	return vars->data[index];
}

WChar *String::data()
{
	if (!vars)
	{
		staticZeroWChar = 0;
		return &staticZeroWChar;
	}
	else return vars->data;
}

Char *String::dataA()
{
	if (isEmpty())
	{
		staticZeroChar = 0;
		return &staticZeroChar;
	}

	if (vars->bufferA) delete[] vars->bufferA;

	UInt length = vars->length;
	UInt bufferASize = (length + 1) * sizeof(WChar);
	vars->bufferA = new Char[bufferASize];
	UInt dstLength = wCharToChar(vars->data, length, vars->bufferA, bufferASize);

	if (dstLength >= bufferASize) vars->bufferA[bufferASize - 1] = 0;
	else vars->bufferA[dstLength] = 0;

	return vars->bufferA;
}

Char *String::dataA(UInt& len)
{
	if (isEmpty())
	{
		len = 0;
		staticZeroChar = 0;
		return &staticZeroChar;
	}

	if (vars->bufferA) delete[] vars->bufferA;

	UInt length = vars->length;
	UInt bufferASize = (length + 1) * sizeof(WChar);
	vars->bufferA = new Char[bufferASize];
	UInt dstLength = wCharToChar(vars->data, length, vars->bufferA, bufferASize);

	if (dstLength < bufferASize)
	{
		vars->bufferA[dstLength] = 0;
		len = dstLength;
	}
	else
	{
		vars->bufferA[bufferASize - 1] = 0;
		len = lengthChar(vars->bufferA);
	}

	return vars->bufferA;
}

Array<WChar> String::dataArray()
{
	if (isEmpty()) return Array<WChar>();

	UInt length = vars->length;
	WChar *data = vars->data;

	Array<WChar> arr(length);
	WChar *arrData = arr.data();
	for (UInt i = 0; i < arr.size(); i++) arrData[i] = data[i];
	return arr;
}

UInt String::length()
{
	return vars ? vars->length : 0;
}

UInt String::size()
{
	return vars ? vars->size : 0;
}

Bool String::isEmpty()
{
	return !vars || vars->length == 0/* like createWithSize, updateLength, etc. */;
}

void String::operator +=(String append)
{
	if (append.isEmpty()) return;

	if (!vars)
	{
		setVars(new StringVars((append.length() + 1) * 2), TRUE);
	}

	UInt originLength = vars->length;
	UInt appendLength = append.vars->length;
	WChar *appendData = append.vars->data;

	UInt resultSize = originLength + appendLength + 1;
	if (resultSize > vars->size)
	{
		WChar *dstData = new WChar[resultSize * 2];
		for (UInt i = 0; i < originLength; i++) dstData[i] = vars->data[i];
		delete[] vars->data;
		vars->data = dstData;
		vars->size = resultSize * 2;
	}

	WChar *dstData = vars->data;
	for (UInt iSrc = 0, iDst = originLength; iSrc < appendLength; iSrc++, iDst++) dstData[iDst] = appendData[iSrc];
	dstData[resultSize - 1] = 0;
	vars->length = originLength + appendLength;
}

String String::operator +(String append)
{
	if (isEmpty()) return append.clone();
	if (append.isEmpty()) return clone();

	UInt originLength = vars->length;
	UInt appendLength = append.vars->length;
	WChar *originData = vars->data;
	WChar *appendData = append.vars->data;

	String out = String::createWithSize(originLength + appendLength + 1);
	WChar *outData = out.data();
	
	for (UInt i = 0; i < originLength; i++) outData[i] = originData[i];
	for (UInt iSrc = 0, iDst = originLength; iSrc < appendLength; iSrc++, iDst++) outData[iDst] = appendData[iSrc];
	outData[originLength + appendLength] = 0;
	out.vars->length = originLength + appendLength;

	return out;
}

Optional<Int> String::toInt()
{
	if (isEmpty()) return Optional<Int>();

	Int val;
	if (scanFromString(vars->data, vars->size, L"%d", &val)) return val;
	else return Optional<Int>();
}

Optional<Long> String::toLong()
{
	if (isEmpty()) return Optional<Long>();

	Long val;
	if (scanFromString(vars->data, vars->size, L"%lld", &val)) return val;
	else return Optional<Long>();
}

Optional<Float> String::toFloat()
{
	if (isEmpty()) return Optional<Float>();

	Float val;
	if (scanFromString(vars->data, vars->size, L"%f", &val)) return val;
	else return Optional<Float>();
}

Optional<Double> String::toDouble()
{
	if (isEmpty()) return Optional<Double>();

	Double val;
	if (scanFromString(vars->data, vars->size, L"%lf", &val)) return val;
	else return Optional<Double>();
}

String String::toUpper()
{
	if (isEmpty()) return String();

	String out = clone();
	WChar *data = out.vars->data;
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
	WChar *data = out.vars->data;
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

	WChar *srcData = vars->data;
	WChar *targetData = target.vars->data;
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

	WChar *srcData = vars->data;
	WChar *targetData = target.vars->data;
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

	UInt bufferSize = (vars->length + 1) * 4; // UTF-8
	Binary out(bufferSize);

	UInt dstLength = wCharToUTF8(vars->data, vars->length, (Char*)out.data(), bufferSize);

	if (dstLength >= bufferSize)
	{
		Char *outData = (Char*)out.data();
		outData[bufferSize - 1] = 0;
		dstLength = lengthChar(outData);
	}

	out.trim(dstLength);
	return out;
}

Array<UInt> String::search(String string)
{
	if (isEmpty()) return Array<UInt>();

	SPADAS_ERROR_RETURNVAL(string.isEmpty(), Array<UInt>());

	UInt subLength = string.vars->length;
	WChar *subData = string.vars->data;

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
		out.initialize(index++, String(*this, Region(matches[i] + length, max((Int)matches[i+1] - (Int)matches[i] - (Int)length, 0))));
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

	WChar *srcStringData = vars->data;
	WChar *newStringData = newString.data(); // vars可能为空
	WChar *outData = out.vars->data;

	UInt outIndex = 0;
	if (matches[0] != 0)
	{
		utility::memoryCopy(&srcStringData[0], &outData[outIndex], matches[0] * sizeof(WChar));
		outIndex += matches[0];
	}
	
	for (UInt i = 0; i < nMatches - 1; i++)
	{
		if (newStringLength != 0)
		{
			utility::memoryCopy(newStringData, &outData[outIndex], newStringLength * sizeof(WChar));
			outIndex += newStringLength;
		}
		
		UInt copySize = max((Int)matches[i+1] - (Int)matches[i] - (Int)oldStringLength, 0);
		if (copySize != 0)
		{
			utility::memoryCopy(&srcStringData[matches[i] + oldStringLength], &outData[outIndex], copySize * sizeof(WChar));
			outIndex += copySize;
		}
	}
	
	if (newStringLength != 0)
	{
		utility::memoryCopy(newStringData, &outData[outIndex], newStringLength * sizeof(WChar));
		outIndex += newStringLength;
	}
	
	UInt copySize = max((Int)srcStringLength - (Int)matches[nMatches-1] - (Int)oldStringLength, 0);
	if (copySize != 0)
	{
		utility::memoryCopy(&srcStringData[matches[nMatches-1] + oldStringLength], &outData[outIndex], copySize * sizeof(WChar));
		outIndex += copySize;
	}
	
	outData[outIndex] = 0;
	return out;
}
