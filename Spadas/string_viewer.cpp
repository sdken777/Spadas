
#include "string_spadas.h"

namespace spadas
{
    class StringViewerVars : public Vars
    {
    public:
        SPADAS_VARS(StringViewer, Vars)
        String source;
        StringSpan span;
        UInt curIndex;
    };

    struct StringSpanStruct
    {
        ULong source;
        UInt idx;
        UInt len;
    };
}

using namespace spadas;

StringViewer::StringViewer() : BaseObject(new StringViewerVars)
{
    var()->curIndex = 0;
}
StringViewer::StringViewer(String source) : BaseObject(new StringViewerVars)
{
    reset(source);
}
void StringViewer::reset(String source)
{
    var()->source = source;
    var()->span = StringSpan();
    if (!source.isEmpty()) 
    {
        StringVars *sourceVars = source.var();
        sourceVars->retain();
        ((StringSpanStruct*)&var()->span)->source = (ULong)sourceVars;
        ((StringSpanStruct*)&var()->span)->idx = 0x80000000; // refCounting flag
        ((StringSpanStruct*)&var()->span)->len = 0;
    }
    var()->curIndex = 0;
}
Bool StringViewer::hasNext()
{
    return var()->curIndex < var()->source.length();
}
StringSpan& StringViewer::next(Char splitter)
{
    UInt sourceLength = var()->source.length();
    SPADAS_ERROR_RETURNVAL(var()->curIndex >= sourceLength, var()->span);

    const Byte *sourceData = var()->source.bytes();
    const Byte *curPos = sourceData + var()->curIndex;
    const Byte *endPos = sourceData + sourceLength;
    const Byte *targetPos = curPos;

    Bool matchBreak = FALSE;
    while (targetPos < endPos)
    {
        if (*targetPos == (Byte)splitter)
        {
            matchBreak = TRUE;
            break;
        }
        else targetPos++;
    }

    ((StringSpanStruct*)&var()->span)->idx = var()->curIndex | 0x80000000;
    if (matchBreak)
    {
        ((StringSpanStruct*)&var()->span)->len = targetPos - curPos;
        var()->curIndex = (UInt)(targetPos - sourceData + 1);
    }
    else
    {
        ((StringSpanStruct*)&var()->span)->len = sourceLength - var()->curIndex;
        var()->curIndex = sourceLength;
    }
    return var()->span;
}
StringSpan& StringViewer::next(String splitter)
{
    SPADAS_ERROR_RETURNVAL(splitter.isEmpty(), var()->span);
    
    UInt subLength = splitter.length();
    const Byte *subData = splitter.bytes();
    if (subLength == 1) return next((Char)subData[0]);

    UInt sourceLength = var()->source.length();
    SPADAS_ERROR_RETURNVAL(var()->curIndex >= sourceLength, var()->span);

    const Byte *sourceData = var()->source.bytes();
    const Byte *curPos = sourceData + var()->curIndex;
    const Byte *endPos = sourceData + sourceLength + 1 - subLength;
    const Byte *targetPos = curPos;
    
    Bool matchBreak = FALSE;
    while (targetPos < endPos)
    {
        Bool match = TRUE;
        for (UInt n = 0; n < subLength; n++)
        {
            if (targetPos[n] != subData[n])
            {
                match = FALSE;
                break;
            }
        }
        if (match)
        {
            matchBreak = TRUE;
            break;
        }
        else targetPos++;
    }

    ((StringSpanStruct*)&var()->span)->idx = var()->curIndex | 0x80000000;
    if (matchBreak)
    {
        ((StringSpanStruct*)&var()->span)->len = targetPos - curPos;
        var()->curIndex = (UInt)(targetPos - sourceData + subLength);
    }
    else
    {
        ((StringSpanStruct*)&var()->span)->len = sourceLength - var()->curIndex;
        var()->curIndex = sourceLength;
    }
    return var()->span;
}
