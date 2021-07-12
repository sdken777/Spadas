
namespace spadas
{
	const UInt MEMCPY_THRESH = 16;

	class BinaryVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(Binary, Vars)

		UInt size;
		Byte *data;
		BinaryVars* binded;
		BinaryVars(UInt size0, Byte* data0) : size(size0), data(data0), binded(NULL)
		{ }
		BinaryVars(UInt size0, Byte* data0, BinaryVars* binded0) : size(size0), data(data0), binded(binded0)
		{
			binded->retain();
		}
		~BinaryVars()
		{
			if (binded) binded->release();
		}
	};
}