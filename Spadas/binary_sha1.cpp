
#include "3party/SHA1.h"
#include "spadas.h"
#include "binary.h"

using namespace binary_internal;

Binary BinaryCommon::toSHA1(const Byte* bytes, UInt size)
{
	if (size == 0) return Binary();

	CSHA1 sha1;
	sha1.Update(bytes, size);
	sha1.Final();

	Binary encoded(20);
	sha1.GetHash(encoded.data());
	return encoded;
}