
#if defined(SPADAS_DEBUG)
#undef SPADAS_DEBUG
#endif

#include "spadas.h"
#include "binary.h"
#include "3party/SHA1.h"

using namespace spadas;

Binary Binary::toSHA1()
{
	if (!vars) return Binary();

	CSHA1 sha1;
	sha1.Update(vars->data, vars->size);
	sha1.Final();

	Binary encoded(20);
	sha1.GetHash(encoded.data());
	return encoded;
}