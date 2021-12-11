
#include "spadas.h"

#include "string_encoding.h"

#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

char hexCharacter(int value)
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

void genhex(char value, char text[3])
{
	text[0] = hexCharacter((int)((value & 0xf0) >> 4));
	text[1] = hexCharacter((int)((value & 0x0f) >> 0));
	text[2] = 0;
}

void genhex(int value, char text[9])
{
	text[0] = hexCharacter((value & 0xf0000000) >> 28);
	text[1] = hexCharacter((value & 0x0f000000) >> 24);
	text[2] = hexCharacter((value & 0x00f00000) >> 20);
	text[3] = hexCharacter((value & 0x000f0000) >> 16);
	text[4] = hexCharacter((value & 0x0000f000) >> 12);
	text[5] = hexCharacter((value & 0x00000f00) >> 8);
	text[6] = hexCharacter((value & 0x000000f0) >> 4);
	text[7] = hexCharacter((value & 0x0000000f) >> 0);
	text[8] = 0;
}

/* --------------------------------------------------------------------- */

typedef unsigned int UTF32;  /* at least 32 bits */
typedef unsigned short UTF16;  /* at least 16 bits */
typedef unsigned char UTF8;   /* typically 8 bits */
typedef unsigned char Boolean; /* 0 or 1 */

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

enum ConversionResult
{
    conversionOK, /* conversion successful */
    sourceExhausted, /* partial character in source, but hit end */
    targetExhausted, /* insuff. room in target for conversion */
    sourceIllegal /* source sequence is illegal/malformed */
};

enum ConversionFlags
{
    strictConversion    = 0,
    lenientConversion
};

#define UNI_SUR_HIGH_START  (UTF32)0xD800
#define UNI_SUR_HIGH_END    (UTF32)0xDBFF
#define UNI_SUR_LOW_START   (UTF32)0xDC00
#define UNI_SUR_LOW_END     (UTF32)0xDFFF

/* --------------------------------------------------------------------- */

static const char trailingBytesForUTF8[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4,
    4, 4, 5, 5, 5, 5
};

static const UTF32 offsetsFromUTF8[6] =
{
    0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL,
    0x82082080UL
};

static const UTF8 firstByteMark[7] =
{
    0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};

/* --------------------------------------------------------------------- */

static Boolean isLegalUTF8(const UTF8* source, int length)
{
    UTF8 a;
    const UTF8* srcptr = source + length;
    switch (length)
    {
        default:
            return false;
            /* Everything else falls through when "true"... */
        case 4:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;
        case 3:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;
        case 2:
            if ((a = (*--srcptr)) > 0xBF)
                return false;
            
            switch (*source)
        {
                /* no fall-through in this inner switch */
            case 0xE0:
                if (a < 0xA0)
                    return false;
                break;
            case 0xED:
                if (a > 0x9F)
                    return false;
                break;
            case 0xF0:
                if (a < 0x90)
                    return false;
                break;
            case 0xF4:
                if (a > 0x8F)
                    return false;
                break;
            default:
                if (a < 0x80)
                    return false;
        }
            
        case 1:
            if (*source >= 0x80 && *source < 0xC2)
                return false;
    }
    if (*source > 0xF4)
        return false;
    return true;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF8(const UTF32** sourceStart,
                                    const UTF32* sourceEnd, UTF8** targetStart, UTF8* targetEnd,
                                    ConversionFlags flags)
{
    ConversionResult result = conversionOK;
    const UTF32* source = *sourceStart;
    UTF8* target = *targetStart;
    while (source < sourceEnd)
    {
        UTF32 ch;
        unsigned short bytesToWrite = 0;
        const UTF32 byteMask = 0xBF;
        const UTF32 byteMark = 0x80;
        ch = *source++;
        if (flags == strictConversion)
        {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
            {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /*
         * Figure out how many bytes the result will require. Turn any
         * illegally large UTF32 things (> Plane 17) into replacement chars.
         */
        if (ch < (UTF32) 0x80)
        {
            bytesToWrite = 1;
        }
        else if (ch < (UTF32) 0x800)
        {
            bytesToWrite = 2;
        }
        else if (ch < (UTF32) 0x10000)
        {
            bytesToWrite = 3;
        }
        else if (ch <= UNI_MAX_LEGAL_UTF32)
        {
            bytesToWrite = 4;
        }
        else
        {
            bytesToWrite = 3;
            ch = UNI_REPLACEMENT_CHAR;
            result = sourceIllegal;
        }
        
        target += bytesToWrite;
        if (target > targetEnd)
        {
            --source; /* Back up source pointer! */
            target -= bytesToWrite; result = targetExhausted; break;
        }
        switch (bytesToWrite)
        {
                /* note: everything falls through. */
            case 4:
                *--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
            case 3:
                *--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
            case 2:
                *--target = (UTF8) ((ch | byteMark) & byteMask); ch >>= 6;
            case 1:
                *--target = (UTF8) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF32(const UTF8** sourceStart,
                                    const UTF8* sourceEnd, UTF32** targetStart, UTF32* targetEnd,
                                    ConversionFlags flags)
{
    ConversionResult result = conversionOK;
    const UTF8* source = *sourceStart;
    UTF32* target = *targetStart;
    while (source < sourceEnd)
    {
        UTF32 ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (source + extraBytesToRead >= sourceEnd)
        {
            result = sourceExhausted; break;
        }
        /* Do this check whether lenient or strict */
        if (!isLegalUTF8(source, extraBytesToRead + 1))
        {
            result = sourceIllegal;
            break;
        }
        /*
         * The cases all fall through. See "Note A" below.
         */
        switch (extraBytesToRead)
        {
            case 5:
                ch += *source++; ch <<= 6;
            case 4:
                ch += *source++; ch <<= 6;
            case 3:
                ch += *source++; ch <<= 6;
            case 2:
                ch += *source++; ch <<= 6;
            case 1:
                ch += *source++; ch <<= 6;
            case 0:
                ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];
        
        if (target >= targetEnd)
        {
            source -= (extraBytesToRead + 1); /* Back up the source pointer! */
            result = targetExhausted; break;
        }
        if (ch <= UNI_MAX_LEGAL_UTF32)
        {
            /*
             * UTF-16 surrogate values are illegal in UTF-32, and anything
             * over Plane 17 (> 0x10FFFF) is illegal.
             */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END)
            {
                if (flags == strictConversion)
                {
                    source -= (extraBytesToRead + 1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
                else
                {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            }
            else
            {
                *target++ = ch;
            }
        }
        else
        {
            /* i.e., ch > UNI_MAX_LEGAL_UTF32 */
            result = sourceIllegal;
            *target++ = UNI_REPLACEMENT_CHAR;
        }
    }
    *sourceStart = source;
    *targetStart = target;
    return result;
}

/////////////////////////////////////////////////////////////////
int string_internal::cvtUTF8toUTF32(const char src[], int srcLength, wchar_t dst[], unsigned int dstSize)
{
	bool srcCopied = false;
    if (srcLength == -1) srcLength = (int)strlen(src);
    else if (src[srcLength] != 0)
    {
    	char *src2 = new char[srcLength+1];
    	for (int i = 0; i < srcLength; i++) src2[i] = src[i];
    	src2[srcLength] = 0;
    	src = src2;
    	srcCopied = true;
    }
    
    const UTF8* a = (const UTF8*)&src[0];
    const UTF8* b = (const UTF8*)&src[srcLength+1];
    UTF32* c = (UTF32*)&dst[0];
    UTF32* d = (UTF32*)&dst[dstSize];
    
    ConvertUTF8toUTF32(&a, b, &c, d, strictConversion);
    dst[dstSize-1] = 0;
    
    if (srcCopied) delete[] src;

    return (int)((unsigned long long)c - (unsigned long long)dst) / 4 - 1;
}

int string_internal::cvtUTF32toUTF8(const wchar_t src[], int srcLength, char dst[], unsigned int dstSize)
{
	bool srcCopied = false;
    if (srcLength == -1) srcLength = (int)wcslen(src);
    else if (src[srcLength] != 0)
    {
    	wchar_t *src2 = new wchar_t[srcLength+1];
    	for (int i = 0; i < srcLength; i++) src2[i] = src[i];
    	src2[srcLength] = 0;
    	src = src2;
    	srcCopied = true;
    }
    
    const UTF32* a = (const UTF32*)&src[0];
    const UTF32* b= (const UTF32*)&src[srcLength+1];
    UTF8* c = (UTF8*)&dst[0];
    UTF8* d = (UTF8*)&dst[dstSize];
    
    ConvertUTF32toUTF8(&a, b, &c, d, strictConversion);
    dst[dstSize-1] = 0;
    
    if (srcCopied) delete[] src;

    return (int)((unsigned long long)c - (unsigned long long)dst) - 1;
}

int string_internal::cvtASCIItoUTF32(const char src[], int srcLength, wchar_t dst[], unsigned int dstSize)
{
    if (srcLength == -1) srcLength = (int)strlen(src);
    
    int dstLength = (srcLength > (int)dstSize - 1) ? ((int)dstSize - 1) : srcLength;
	for (int i = 0; i < dstLength; i++)
	{
		if (src[i] > 0) dst[i] = (wchar_t)src[i];
		else dst[i] = L'?';
	}
	dst[dstLength] = 0;
    
    return dstLength;
}

int string_internal::cvtUTF32toASCII(const wchar_t src[], int srcLength, char dst[], unsigned int dstSize)
{
    if (srcLength == -1) srcLength = (int)wcslen(src);
    
    int dstLength = (srcLength > (int)dstSize - 1) ? ((int)dstSize - 1) : srcLength;
	for (int i = 0; i < dstLength; i++)
	{
		if (src[i] > 0 && src[i] < 128) dst[i] = (char)src[i];
		else dst[i] = '?';
	}
	dst[dstLength] = 0;
    
    return dstLength;
}

#endif
