
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

	class BinaryCommon
	{
	public:
		static Binary clone(const Byte* bytes, UInt size);
		static Binary operatorPlus(const Byte* bytes, UInt size, Binary& append);
		static Array<BinarySpan> split(const Byte* bytes, UInt size, Array<UInt>& sizes, Vars *binaryVars);
		static Binary reverse(const Byte* bytes, UInt size);
		static String toBase64(const Byte* bytes, UInt size);
		static Binary toSHA1(const Byte* bytes, UInt size);
		static Binary toDES(const Byte* bytes, UInt size, String& key);
	};
}