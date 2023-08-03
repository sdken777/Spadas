
#include "binary.h"
#include "3party/base64.h"

using namespace binary_internal;

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

String BinaryCommon::toBase64(const Byte* bytes, UInt size)
{
	if (size == 0) return String();

	UInt numOrig24BitValues = size / 3;
	Bool havePadding = size > numOrig24BitValues * 3;
	UInt numResultBytes = 4 * (numOrig24BitValues + havePadding) + 1;
	
	Array<Char> chars(numResultBytes, 0);
	Char *endPtr = bintob64(chars.data(), bytes, size);
	if (!endPtr) return String();

	*endPtr = 0;
	return chars;
}