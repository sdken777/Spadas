
#include "string_spadas.h"
#include <stdlib.h>
#undef NULL
#define NULL 0

namespace string_internal
{
	Array<Char> emptyChars = Array<Char>::scalar(0);
	Array<WChar> emptyWChars = Array<WChar>::scalar(0);

	Bool stringToInt(const Byte *src, UInt len, Byte *buf, Int& out)
	{
		utility::memoryCopy(src, buf, len);
		buf[len] = 0;
		Char *endPtr = NULL;
		out = (Int)strtol((Char*)buf, &endPtr, 10);
		return endPtr - (Char*)buf == len;
	}

	Bool stringToLong(const Byte *src, UInt len, Byte *buf, Long& out)
	{
		utility::memoryCopy(src, buf, len);
		buf[len] = 0;
		Char *endPtr = NULL;
		out = strtoll((Char*)buf, &endPtr, 10);
		return endPtr - (Char*)buf == len;
	}

	Bool stringToFloat(const Byte *src, UInt len, Byte *buf, Float& out)
	{
		utility::memoryCopy(src, buf, len);
		buf[len] = 0;
		Char *endPtr = NULL;
		out = strtof((Char*)buf, &endPtr);
		return endPtr - (Char*)buf == len;
	}

	Bool stringToDouble(const Byte *src, UInt len, Byte *buf, Double& out)
	{
		utility::memoryCopy(src, buf, len);
		buf[len] = 0;
		Char *endPtr = NULL;
		out = strtod((Char*)buf, &endPtr);
		return endPtr - (Char*)buf == len;
	}
}

using namespace spadas;
using namespace string_internal;

String StringCommon::clone()
{
	UInt len = length();
	if (len == 0) return String();

	String out = String::createWithSize(len);
	utility::memoryCopy(bytes(), out.getVars()->data, len);
	out.getVars()->length = len;
	return out;
}

Array<Char> StringCommon::chars()	
{
	UInt len = length();
	if (len == 0) return emptyChars;

	Array<Char> output(len + 1);
	utility::memoryCopy(bytes(), output.data(), len);
	output[len] = 0;
	return output;
}

Array<WChar> StringCommon::wchars()
{
	UInt len = length();
	if (len == 0) return emptyWChars;

	Array<WChar> output(len + 1);
	UInt outputLength = utf8ToWChar((Char*)bytes(), len, output.data(), output.size());
	output.trim(outputLength + 1);
	return output;
}

Bool StringCommon::isEmpty()
{
	return length() == 0;
}

StringAppender StringCommon::operator +(String append)
{
	UInt originLength = length();
	UInt appendLength = append.length();
	UInt totalLength = originLength + appendLength;
	if (totalLength == 0) return String();

	String out = String::createWithSize(totalLength * 2);
	if (originLength != 0) utility::memoryCopy(bytes(), out.getVars()->data, originLength);
	if (appendLength != 0) utility::memoryCopy(append.getVars()->data, &out.getVars()->data[originLength], appendLength);
	out.getVars()->length = totalLength;
	return out;
}

Optional<Int> StringCommon::toInt()
{
	UInt len = length();
	if (len == 0 || len >= 12) return Optional<Int>();

	Byte buffer[12];
	Int val;
	Bool ok = stringToInt(bytes(), len, buffer, val);
	return ok ? val : Optional<Int>();
}

Bool StringCommon::toNumber(Int& number)
{
	UInt len = length();
	if (len == 0 || len >= 12) return FALSE;

	Byte buffer[12];
	Int val;
	Bool ok = stringToInt(bytes(), len, buffer, val);
	if (ok) number = val;
	return ok;
}

Optional<Long> StringCommon::toLong()
{
	UInt len = length();
	if (len == 0 || len >= 22) return Optional<Long>();

	Byte buffer[22];
	Long val;
	Bool ok = stringToLong(bytes(), len, buffer, val);
	return ok ? val : Optional<Long>();
}

Bool StringCommon::toNumber(Long& number)
{
	UInt len = length();
	if (len == 0 || len >= 22) return FALSE;

	Byte buffer[22];
	Long val;
	Bool ok = stringToLong(bytes(), len, buffer, val);
	if (ok) number = val;
	return ok;
}

Optional<Float> StringCommon::toFloat()
{
	UInt len = length();
	if (len == 0) return Optional<Float>();

	Float val = 0;
	Bool ok = FALSE;
	if (len < 16)
	{
		Byte buffer[16];
		ok = stringToFloat(bytes(), len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToFloat(bytes(), len, buffer.data(), val);
	}
	return ok ? val : Optional<Float>();
}

Bool StringCommon::toNumber(Float& number)
{
	UInt len = length();
	if (len == 0) return FALSE;

	Float val = 0;
	Bool ok = FALSE;
	if (len < 16)
	{
		Byte buffer[16];
		ok = stringToFloat(bytes(), len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToFloat(bytes(), len, buffer.data(), val);
	}
	if (ok) number = val;
	return ok;
}

Optional<Double> StringCommon::toDouble()
{
	UInt len = length();
	if (len == 0) return Optional<Double>();

	Double val = 0;
	Bool ok = FALSE;
	if (len < 24)
	{
		Byte buffer[24];
		ok = stringToDouble(bytes(), len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToDouble(bytes(), len, buffer.data(), val);
	}
	return ok ? val : Optional<Double>();
}

Bool StringCommon::toNumber(Double& number)
{
	UInt len = length();
	if (len == 0) return FALSE;

	Double val = 0;
	Bool ok = FALSE;
	if (len < 24)
	{
		Byte buffer[24];
		ok = stringToDouble(bytes(), len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToDouble(bytes(), len, buffer.data(), val);
	}
	if (ok) number = val;
	return ok;
}

String StringCommon::toUpper()
{
	UInt len = length();
	if (len == 0) return String();

	String out = clone();
	Byte *data = out.getVars()->data;
	for (UInt i = 0; i < len; i++)
	{
		if (data[i] >= 97 && data[i] <= 122) data[i] -= 32;
	}
	return out;
}

String StringCommon::toLower()
{
	UInt len = length();
	if (len == 0) return String();

	String out = clone();
	Byte *data = out.getVars()->data;
	for (UInt i = 0; i < len; i++)
	{
		if (data[i] >= 65 && data[i] <= 90) data[i] += 32;
	}
	return out;
}

Bool StringCommon::startsWith(String target)
{
	UInt srcLength = length();
	UInt targetLength = target.length();
	if (srcLength == 0 || targetLength == 0) return FALSE;
	if (targetLength > srcLength) return FALSE;

	const Byte *srcData = bytes();
	Byte *targetData = target.getVars()->data;
	for (UInt i = 0; i < targetLength; i++)
	{
		if (srcData[i] != targetData[i]) return FALSE;
	}
	return TRUE;
}

Bool StringCommon::endsWith(String target)
{
	UInt srcLength = length();
	UInt targetLength = target.length();
	if (srcLength == 0 || targetLength == 0) return FALSE;
	if (targetLength > srcLength) return FALSE;

	const Byte *srcData = bytes();
	Byte *targetData = target.getVars()->data;
	UInt offset = srcLength - targetLength;
	for (UInt i = 0; i < targetLength; i++)
	{
		if (srcData[offset + i] != targetData[i]) return FALSE;
	}
	return TRUE;
}

Binary StringCommon::toBinary()
{
	UInt len = length();
	if (len == 0) return Binary();
	else return Binary(bytes(), len);
}

Array<UInt> StringCommon::search(String string)
{
	UInt srcLength = length();
	if (srcLength == 0) return Array<UInt>();

	UInt subLength = string.length();
	SPADAS_ERROR_RETURNVAL(subLength == 0, Array<UInt>());
	if (subLength > srcLength) return Array<UInt>();

	const Byte *srcData = bytes();
	Byte *subData = string.getVars()->data;

	if (srcLength >= 64)
	{
		ArrayX<UInt> bufIndices(srcLength >= 8192 ? (srcLength >= 1048576 ? 256 : 64) : 16);
		for (UInt i = 0; i <= srcLength - subLength; i++)
		{
			Bool match = TRUE;
			for (UInt n = 0, j = i; n < subLength; n++, j++)
			{
				if (srcData[j] != subData[n])
				{
					match = FALSE;
					break;
				}
			}
			if (match) bufIndices.append(i);
		}
		return bufIndices.toArray();
	}
	else
	{
		Array<UInt> bufIndices(srcLength);
		UInt nBufIndices = 0;
		for (UInt i = 0; i <= srcLength - subLength; i++)
		{
			Bool match = TRUE;
			for (UInt n = 0, j = i; n < subLength; n++, j++)
			{
				if (srcData[j] != subData[n])
				{
					match = FALSE;
					break;
				}
			}
			if (match) bufIndices[nBufIndices++] = i;
		}
		if (nBufIndices == 0) return Array<UInt>();
		bufIndices.trim(nBufIndices);
		return bufIndices;
	}
}

Array<StringSpan> StringCommon::split(String string)
{
	UInt srcLength = length();
	if (srcLength == 0) return Array<StringSpan>();

	UInt splitterLength = string.length();
	SPADAS_ERROR_RETURNVAL(splitterLength == 0, Array<StringSpan>());

	Array<UInt> rawMatches = search(string);
	
	UInt nRawMatches = rawMatches.size();
	if (nRawMatches == 0) return Array<StringSpan>::scalar(genStringSpan(0, srcLength));
	
	Array<UInt> matchesArr(nRawMatches);
	UInt *matches = matchesArr.data();
	UInt nMatches = 0;
	matches[nMatches++] = rawMatches[0];
	for (UInt i = 1; i < nRawMatches; i++)
	{
		if (rawMatches[i] < matches[nMatches - 1] + splitterLength) continue;
		matches[nMatches++] = rawMatches[i];
	}
	
	UInt outSize = nMatches + 1;
	Array<StringSpan> out = Array<StringSpan>::createUninitialized(outSize);

	UInt index = 0;
	StringSpan dummySpan;

	if (matches[0] > 0) out.initialize(index++, genStringSpan(0, matches[0]));
	else out.initialize(index++, dummySpan);

	for (UInt i = 0; i < nMatches - 1; i++)
	{
		UInt spanLength = math::max((Int)matches[i+1] - (Int)matches[i] - (Int)splitterLength, 0);
		if (spanLength > 0) out.initialize(index++, genStringSpan(matches[i] + splitterLength, spanLength));
		else out.initialize(index++, dummySpan);
	}

	UInt lastIndex = matches[nMatches-1] + splitterLength;
	if (lastIndex >= srcLength) out.initialize(index++, dummySpan);
	else out.initialize(index++, genStringSpan(lastIndex, srcLength - lastIndex));
	
	return out;
}

String StringCommon::replace(String oldString, String newString)
{
	UInt srcStringLength = length();
	if (srcStringLength == 0) return String();

	UInt oldStringLength = oldString.length();
	SPADAS_ERROR_RETURNVAL(oldStringLength == 0, clone());

	UInt newStringLength = newString.length();
	
	Array<UInt> rawMatches = search(oldString);
	
	UInt nRawMatches = rawMatches.size();
	if (nRawMatches == 0) return clone();
	
	Array<UInt> matchesArr(nRawMatches);
	UInt *matches = matchesArr.data();
	UInt nMatches = 0;
	matches[nMatches++] = rawMatches[0];
	for (UInt i = 1; i < nRawMatches; i++)
	{
		if (rawMatches[i] < matches[nMatches-1] + oldStringLength) continue;
		matches[nMatches++] = rawMatches[i];
	}

	UInt outLength = srcStringLength + nMatches * newStringLength - nMatches * oldStringLength;
	if (outLength == 0) return String();
	
	String out = String::createWithSize(outLength);
	out.getVars()->length = outLength;

	const Byte *srcStringData = bytes();
	Byte *newStringData = newStringLength == 0 ? NULL : newString.getVars()->data;
	Byte *outData = out.getVars()->data;

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

	return out;
}

StringSpan StringCommon::subString(UInt index, UInt length, Bool trimStart, Bool trimEnd)
{
	StringSpan span = genStringSpan(index, length);
	if (!trimStart && !trimEnd) return span;

	UInt rawLength = span.length();
	if (rawLength == 0) return StringSpan();

	UInt trimIndex = 0, trimLength = rawLength;
	const Byte* data = span.bytes();
	if (trimStart)
	{
		for (UInt i = 0; i < rawLength; i++)
		{
			if (data[i] != ' ') break;
			trimIndex++;
		}
		if (trimIndex == rawLength) return StringSpan();
		trimLength -= trimIndex;
	}
	if (trimEnd)
	{
		for (Int i = (Int)rawLength - 1; i > (Int)trimIndex; i--)
		{
			if (data[i] != ' ') break;
			trimLength--;
		}
	}
	return ((StringCommon&)span).genStringSpan(trimIndex, trimLength);
}

Word StringCommon::getHashCode()
{
	UInt len = length();
	if (len == 0) return 0;
	
	const Byte *data = bytes();
	UInt hash = 0;
	for (UInt i = 0; i < len; i++)
	{
		hash += (UInt)data[i];
		hash *= 3;
	}
	return (Word)(hash & 0x0000ffff);
}

StringCommon::~StringCommon()
{}