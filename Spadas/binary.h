
#include "spadas.h"

namespace spadas
{
	class BinaryVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(Binary, Vars)

		UInt size;
		Byte* data;
		BinaryVars(UInt size0, Byte* data0) : size(size0), data(data0)
		{ }
	};
}

namespace binary_internal
{
	using namespace spadas;

	extern unsigned char* base64Decode(char* in, unsigned int& resultSize, bool trimTrailingZeros);
	extern char* base64Encode(char const* origSigned, unsigned origLength);
	extern bool calc_sha1(const unsigned char *data, unsigned int len, unsigned char *pbDest20);
	extern Optional<Binary> desDecode(Binary encrypted, Binary sha1);
	extern Binary desEncode(Binary sha1, const Byte* data, UInt rawsize);

	class BinaryCommon
	{
	public:
		static Binary clone(const Byte* bytes, UInt size);
		static Binary operatorPlus(const Byte* bytes, UInt size, Binary& append);
		static Array<BinarySpan> split(Binary& source, UInt spanIndex, UInt spanSize, Array<UInt>& sizes);
		static Binary reverse(const Byte* bytes, UInt size);
		static String toBase64(const Byte* bytes, UInt size);
		static Binary toSHA1(const Byte* bytes, UInt size);
		static Binary toDES(const Byte* bytes, UInt size, String& key);
		static BinarySpan sub(Binary& source, UInt spanIndex, UInt spanSize, UInt subIndex, UInt subSize);
	};
}