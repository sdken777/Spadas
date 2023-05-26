
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