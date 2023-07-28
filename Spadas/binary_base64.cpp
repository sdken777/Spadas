
#if defined(SPADAS_DEBUG)
#undef SPADAS_DEBUG
#endif

#include "spadas.h"
#include "binary.h"
#include "3party/base64.h"

using namespace spadas;

Optional<Binary> Binary::createFromBase64(String base64)
{
	if (base64.isEmpty()) return Binary();

	Array<Char> buffer = base64.chars();
	Byte *startPtr = (Byte*)buffer.data();
	Byte *endPtr = (Byte*)b64decode(startPtr);
	if (!endPtr) return Optional<Binary>();
	
	UInt size = endPtr - startPtr;
	return Binary(startPtr, size);
}

String Binary::toBase64()
{
	if (!vars) return String();

	UInt numOrig24BitValues = vars->size / 3;
	Bool havePadding = vars->size > numOrig24BitValues * 3;
	UInt numResultBytes = 4 * (numOrig24BitValues + havePadding) + 1;
	
	Array<Char> chars(numResultBytes, 0);
	Char *endPtr = bintob64(chars.data(), vars->data, vars->size);
	if (!endPtr) return String();

	*endPtr = 0;
	return chars;
}