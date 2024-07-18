
#include <stdlib.h>
#undef NULL

#include "string_spadas.h"

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

Word StringCommon::getHashCode(const Byte* data, UInt len)
{
	if (len == 0) return 0;
	
	UInt hash = 0;
	for (UInt i = 0; i < len; i++)
	{
		hash += (UInt)data[i];
		hash *= 3;
	}
	return (Word)(hash & 0x0000ffff);
}

String StringCommon::clone(const Byte* bytes, UInt len)
{
	if (len == 0) return String();

	String out = String::createWithSize(len);
	utility::memoryCopy(bytes, out.getVars()->data, len);
	out.getVars()->length = len;
	return out;
}

Array<Char> StringCommon::chars(const Byte* bytes, UInt len)	
{
	if (len == 0) return emptyChars;

	Array<Char> output(len + 1);
	utility::memoryCopy(bytes, output.data(), len);
	output[len] = 0;
	return output;
}

Array<WChar> StringCommon::wchars(const Byte* bytes, UInt len)
{
	if (len == 0) return emptyWChars;

	Array<WChar> output(len + 1);
	UInt outputLength = utf8ToWChar((Char*)bytes, len, output.data(), output.size());
	output.trim(outputLength + 1);
	return output;
}

StringAppender StringCommon::operatorPlus(const Byte* bytes, UInt originLength, String& append)
{
	UInt appendLength = append.length();
	UInt totalLength = originLength + appendLength;
	if (totalLength == 0) return String();

	String out = String::createWithSize(totalLength * 2);
	if (originLength != 0) utility::memoryCopy(bytes, out.getVars()->data, originLength);
	if (appendLength != 0) utility::memoryCopy(append.getVars()->data, &out.getVars()->data[originLength], appendLength);
	out.getVars()->length = totalLength;
	return out;
}

StringAppender StringCommon::operatorPlusSpan(const Byte* bytes, UInt originLength, StringSpan& append)
{
	UInt appendLength = append.length();
	UInt totalLength = originLength + appendLength;
	if (totalLength == 0) return String();

	String out = String::createWithSize(totalLength * 2);
	if (originLength != 0) utility::memoryCopy(bytes, out.getVars()->data, originLength);
	if (appendLength != 0) utility::memoryCopy(append.bytes(), &out.getVars()->data[originLength], appendLength);
	out.getVars()->length = totalLength;
	return out;
}

Optional<Int> StringCommon::toInt(const Byte* bytes, UInt len)
{
	if (len == 0 || len >= 12) return Optional<Int>();

	Byte buffer[12];
	Int val;
	Bool ok = stringToInt(bytes, len, buffer, val);
	return ok ? val : Optional<Int>();
}

Int StringCommon::toInt(const Byte* bytes, UInt len, Int defaultValue)
{
	if (len == 0 || len >= 12) return defaultValue;

	Byte buffer[12];
	Int val;
	Bool ok = stringToInt(bytes, len, buffer, val);
	return ok ? val : defaultValue;
}

Bool StringCommon::toNumber(const Byte* bytes, UInt len, Int& number)
{
	if (len == 0 || len >= 12) return FALSE;

	Byte buffer[12];
	Int val;
	Bool ok = stringToInt(bytes, len, buffer, val);
	if (ok) number = val;
	return ok;
}

Optional<Long> StringCommon::toLong(const Byte* bytes, UInt len)
{
	if (len == 0 || len >= 22) return Optional<Long>();

	Byte buffer[22];
	Long val;
	Bool ok = stringToLong(bytes, len, buffer, val);
	return ok ? val : Optional<Long>();
}

Long StringCommon::toLong(const Byte* bytes, UInt len, Long defaultValue)
{
	if (len == 0 || len >= 22) return defaultValue;

	Byte buffer[22];
	Long val;
	Bool ok = stringToLong(bytes, len, buffer, val);
	return ok ? val : defaultValue;
}

Bool StringCommon::toNumber(const Byte* bytes, UInt len, Long& number)
{
	if (len == 0 || len >= 22) return FALSE;

	Byte buffer[22];
	Long val;
	Bool ok = stringToLong(bytes, len, buffer, val);
	if (ok) number = val;
	return ok;
}

Optional<Float> StringCommon::toFloat(const Byte* bytes, UInt len)
{
	if (len == 0) return Optional<Float>();

	Float val = 0;
	Bool ok = FALSE;
	if (len < 16)
	{
		Byte buffer[16];
		ok = stringToFloat(bytes, len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToFloat(bytes, len, buffer.data(), val);
	}
	return ok ? val : Optional<Float>();
}

Float StringCommon::toFloat(const Byte* bytes, UInt len, Float defaultValue)
{
	if (len == 0) return defaultValue;

	Float val = 0;
	Bool ok = FALSE;
	if (len < 16)
	{
		Byte buffer[16];
		ok = stringToFloat(bytes, len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToFloat(bytes, len, buffer.data(), val);
	}
	return ok ? val : defaultValue;
}

Bool StringCommon::toNumber(const Byte* bytes, UInt len, Float& number)
{
	if (len == 0) return FALSE;

	Float val = 0;
	Bool ok = FALSE;
	if (len < 16)
	{
		Byte buffer[16];
		ok = stringToFloat(bytes, len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToFloat(bytes, len, buffer.data(), val);
	}
	if (ok) number = val;
	return ok;
}

Optional<Double> StringCommon::toDouble(const Byte* bytes, UInt len)
{
	if (len == 0) return Optional<Double>();

	Double val = 0;
	Bool ok = FALSE;
	if (len < 24)
	{
		Byte buffer[24];
		ok = stringToDouble(bytes, len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToDouble(bytes, len, buffer.data(), val);
	}
	return ok ? val : Optional<Double>();
}

Double StringCommon::toDouble(const Byte* bytes, UInt len, Double defaultValue)
{
	if (len == 0) return defaultValue;

	Double val = 0;
	Bool ok = FALSE;
	if (len < 24)
	{
		Byte buffer[24];
		ok = stringToDouble(bytes, len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToDouble(bytes, len, buffer.data(), val);
	}
	return ok ? val : defaultValue;
}

Bool StringCommon::toNumber(const Byte* bytes, UInt len, Double& number)
{
	if (len == 0) return FALSE;

	Double val = 0;
	Bool ok = FALSE;
	if (len < 24)
	{
		Byte buffer[24];
		ok = stringToDouble(bytes, len, buffer, val);
	}
	else
	{
		Binary buffer(len + 1);
		ok = stringToDouble(bytes, len, buffer.data(), val);
	}
	if (ok) number = val;
	return ok;
}

String StringCommon::toUpper(const Byte* bytes, UInt len)
{
	if (len == 0) return String();

	String out = clone(bytes, len);
	Byte *data = out.getVars()->data;
	for (UInt i = 0; i < len; i++)
	{
		if (data[i] >= 97 && data[i] <= 122) data[i] -= 32;
	}
	return out;
}

String StringCommon::toLower(const Byte* bytes, UInt len)
{
	if (len == 0) return String();

	String out = clone(bytes, len);
	Byte *data = out.getVars()->data;
	for (UInt i = 0; i < len; i++)
	{
		if (data[i] >= 65 && data[i] <= 90) data[i] += 32;
	}
	return out;
}

Bool StringCommon::startsWith(const Byte* srcData, UInt srcLength, String& target)
{
	UInt targetLength = target.length();
	if (srcLength == 0 || targetLength == 0) return FALSE;
	if (targetLength > srcLength) return FALSE;

	Byte *targetData = target.getVars()->data;
	for (UInt i = 0; i < targetLength; i++)
	{
		if (srcData[i] != targetData[i]) return FALSE;
	}
	return TRUE;
}

Bool StringCommon::endsWith(const Byte* srcData, UInt srcLength, String& target)
{
	UInt targetLength = target.length();
	if (srcLength == 0 || targetLength == 0) return FALSE;
	if (targetLength > srcLength) return FALSE;

	Byte *targetData = target.getVars()->data;
	UInt offset = srcLength - targetLength;
	for (UInt i = 0; i < targetLength; i++)
	{
		if (srcData[offset + i] != targetData[i]) return FALSE;
	}
	return TRUE;
}

Binary StringCommon::toBinary(const Byte* bytes, UInt len)
{
	if (len == 0) return Binary();
	else return Binary(bytes, len);
}

UInt StringCommon::count(const Byte* srcData, UInt srcLength, String& string)
{
	if (srcLength == 0) return 0;

	UInt subLength = string.length();
	SPADAS_ERROR_RETURNVAL(subLength == 0, 0);
	if (subLength > srcLength) return 0;

	Byte *subData = string.getVars()->data;
	const UInt forCount = srcLength - subLength;

    UInt nMatches = 0;
    if (subLength == 1)
    {
        Byte targetByte = subData[0];
        for (UInt i = 0; i <= forCount; i++)
        {
            if (srcData[i] == targetByte) nMatches++;
        }
    }
    else
    {
        for (UInt i = 0; i <= forCount; i++)
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
            if (match) nMatches++;
        }
    }
    return nMatches;
}

Array<UInt> StringCommon::search(const Byte* srcData, UInt srcLength, String& string)
{
	if (srcLength == 0) return Array<UInt>();

	UInt subLength = string.length();
	SPADAS_ERROR_RETURNVAL(subLength == 0, Array<UInt>());
	if (subLength > srcLength) return Array<UInt>();

	Byte *subData = string.getVars()->data;
	const UInt forCount = srcLength - subLength;

	if (srcLength >= 64)
	{
		ArrayX<UInt> bufIndices(srcLength >= 8192 ? (srcLength >= 1048576 ? 256 : 64) : 16);
		if (subLength == 1)
		{
			Byte targetByte = subData[0];
			for (UInt i = 0; i <= forCount; i++)
			{
				if (srcData[i] == targetByte) bufIndices.append(i);
			}
		}
		else
		{
			for (UInt i = 0; i <= forCount; i++)
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
		}
		return bufIndices.toArray();
	}
	else
	{
		Array<UInt> bufIndices(srcLength);
		UInt *bufIndicesData = bufIndices.data();
		UInt nBufIndices = 0;
		if (subLength == 1)
		{
			Byte targetByte = subData[0];
			for (UInt i = 0; i <= forCount; i++)
			{
				if (srcData[i] == targetByte) bufIndicesData[nBufIndices++] = i;
			}
		}
		else
		{
			for (UInt i = 0; i <= forCount; i++)
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
				if (match) bufIndicesData[nBufIndices++] = i;
			}
		}
		if (nBufIndices == 0) return Array<UInt>();
		bufIndices.trim(nBufIndices);
		return bufIndices;
	}
}

UInt StringCommon::searchFirst(const Byte* srcData, UInt srcLength, String& string)
{
	if (srcLength == 0) return UINF;

	UInt subLength = string.length();
	SPADAS_ERROR_RETURNVAL(subLength == 0, UINF);
	if (subLength > srcLength) return UINF;

	Byte *subData = string.getVars()->data;
	const UInt forCount = srcLength - subLength;

	if (subLength == 1)
	{
		Byte targetByte = subData[0];
		for (UInt i = 0; i <= forCount; i++)
		{
			if (srcData[i] == targetByte) return i;
		}
	}
	else
	{
		for (UInt i = 0; i <= forCount; i++)
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
			if (match) return i;
		}
	}
	return UINF;
}

UInt StringCommon::searchLast(const Byte* srcData, UInt srcLength, String& string)
{
	if (srcLength == 0) return UINF;

	UInt subLength = string.length();
	SPADAS_ERROR_RETURNVAL(subLength == 0, UINF);
	if (subLength > srcLength) return UINF;

	Byte *subData = string.getVars()->data;
	if (subLength == 1)
	{
		Byte targetByte = subData[0];
		for (Int i = (Int)(srcLength - subLength); i >= 0; i--)
		{
			if (srcData[i] == targetByte) return i;
		}
	}
	else
	{
		for (Int i = (Int)(srcLength - subLength); i >= 0; i--)
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
			if (match) return i;
		}
	}
	return UINF;
}

String StringCommon::trim(const Byte* srcData, UInt srcLength)
{
	if (srcLength == 0) return String();

	UInt trimIndex = 0, trimLength = srcLength, rawLength = srcLength;

	for (UInt i = 0; i < rawLength; i++)
	{
		Byte c = (Char)srcData[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') break;
		trimIndex++;
	}
	if (trimIndex == rawLength) return String();
	trimLength -= trimIndex;

	for (Int i = (Int)rawLength - 1; i > (Int)trimIndex; i--)
	{
		Byte c = (Char)srcData[i];
		if (c != ' ' && c != '\t' && c != '\r' && c != '\n') break;
		trimLength--;
	}

	return StringSpan(srcData + trimIndex, trimLength).clone();
}

Array<StringSpan> StringCommon::split(const Byte* srcData, UInt srcLength, String& splitter, Vars *stringVars)
{
	if (srcLength == 0) return Array<StringSpan>();

	UInt splitterLength = splitter.length();
	SPADAS_ERROR_RETURNVAL(splitterLength == 0, Array<StringSpan>());

	Array<UInt> rawMatches = search(srcData, srcLength, splitter);
	
    String sourceString;
    UInt curIndex = 0;
    if (stringVars)
    {
        auto objBase = Object<StringVars>::castCreate(stringVars);
		sourceString = *(String*)(&objBase);
        curIndex = (UInt)(srcData - sourceString.bytes());
    }

	UInt nRawMatches = rawMatches.size();
	if (nRawMatches == 0)
	{
		if (stringVars) return Array<StringSpan>::scalar(StringSpan(sourceString, curIndex, srcLength));
		else return Array<StringSpan>::scalar(StringSpan(srcData, srcLength));
	}
	
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

	if (matches[0] > 0) out.initialize(index++, stringVars ? StringSpan(sourceString, curIndex, matches[0]) : StringSpan(srcData, matches[0]));
	else out.initialize(index++, StringSpan());

	for (UInt i = 0; i < nMatches - 1; i++)
	{
		UInt subStringLength = math::max((Int)matches[i+1] - (Int)matches[i] - (Int)splitterLength, 0);
		if (subStringLength > 0) out.initialize(index++, stringVars ? StringSpan(sourceString, curIndex + matches[i] + splitterLength, subStringLength) : StringSpan(srcData + matches[i] + splitterLength, subStringLength));
		else out.initialize(index++, StringSpan());
	}

	UInt lastIndex = matches[nMatches-1] + splitterLength;
	if (lastIndex >= srcLength) out.initialize(index++, StringSpan());
	else out.initialize(index++, stringVars ? StringSpan(sourceString, curIndex + lastIndex, srcLength - lastIndex) : StringSpan(srcData + lastIndex, srcLength - lastIndex));
	
	return out;
}

String StringCommon::replace(const Byte* srcStringData, UInt srcStringLength, String& oldString, String& newString)
{
	if (srcStringLength == 0) return String();

	UInt oldStringLength = oldString.length();
	SPADAS_ERROR_RETURNVAL(oldStringLength == 0, clone(srcStringData, srcStringLength));

	UInt newStringLength = newString.length();
	
	Array<UInt> rawMatches = search(srcStringData, srcStringLength, oldString);
	
	UInt nRawMatches = rawMatches.size();
	if (nRawMatches == 0) return clone(srcStringData, srcStringLength);
	
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