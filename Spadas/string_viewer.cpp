
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

StringViewer::StringViewer() : Object<StringViewerVars>(new StringViewerVars, TRUE)
{
    vars->curIndex = 0;
}
StringViewer::StringViewer(String source) : Object<StringViewerVars>(new StringViewerVars, TRUE)
{
    reset(source);
}
void StringViewer::reset(String source)
{
    vars->source = source;
    vars->span = StringSpan();
    if (!source.isEmpty()) 
    {
        StringVars *sourceVars = source.getVars();
        sourceVars->retain();
        ((StringSpanStruct*)&vars->span)->source = (ULong)sourceVars;
        ((StringSpanStruct*)&vars->span)->idx = 0x80000000; // refCounting flag
        ((StringSpanStruct*)&vars->span)->len = 0;
    }
    vars->curIndex = 0;
}
Bool StringViewer::hasNext()
{
    return vars->curIndex < vars->source.length();
}
StringSpan& StringViewer::next(Char splitter)
{
    UInt sourceLength = vars->source.length();
    SPADAS_ERROR_RETURNVAL(vars->curIndex >= sourceLength, vars->span);

    const Byte *sourceData = vars->source.bytes();
    const Byte *curPos = sourceData + vars->curIndex;
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

    ((StringSpanStruct*)&vars->span)->idx = vars->curIndex | 0x80000000;
    if (matchBreak)
    {
        ((StringSpanStruct*)&vars->span)->len = targetPos - curPos;
        vars->curIndex = (UInt)(targetPos - sourceData + 1);
    }
    else
    {
        ((StringSpanStruct*)&vars->span)->len = sourceLength - vars->curIndex;
        vars->curIndex = sourceLength;
    }
    return vars->span;
}
StringSpan& StringViewer::next(String splitter)
{
    SPADAS_ERROR_RETURNVAL(splitter.isEmpty(), vars->span);
    
    UInt subLength = splitter.length();
    const Byte *subData = splitter.bytes();
    if (subLength == 1) return next((Char)subData[0]);

    UInt sourceLength = vars->source.length();
    SPADAS_ERROR_RETURNVAL(vars->curIndex >= sourceLength, vars->span);

    const Byte *sourceData = vars->source.bytes();
    const Byte *curPos = sourceData + vars->curIndex;
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

    ((StringSpanStruct*)&vars->span)->idx = vars->curIndex | 0x80000000;
    if (matchBreak)
    {
        ((StringSpanStruct*)&vars->span)->len = targetPos - curPos;
        vars->curIndex = (UInt)(targetPos - sourceData + subLength);
    }
    else
    {
        ((StringSpanStruct*)&vars->span)->len = sourceLength - vars->curIndex;
        vars->curIndex = sourceLength;
    }
    return vars->span;
}
