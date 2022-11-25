
namespace spadas
{
	class StringVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(String, Vars)

		UInt size;
		Byte* data;
		UInt length;
		Byte* extData;
		
		// constructors
		StringVars(UInt size0, Byte* data0): size(size0), data(data0), length(0), extData(0)
		{}
        
		// destructor
        ~StringVars()
        {
            if (extData) delete[] extData;
        }

		// to string
		String toString() override
		{
			String str;
			str.setVars(this, FALSE);
			return str;
		}
	};
}

namespace spadas_internal
{
	using namespace spadas;

	Bool doubleToPrettyString(Double val, Char output[48], Int& length);
	
	UInt lengthChar(const Char str[]);
	UInt lengthWChar(const WChar str[]);
	Int compareString(const Char str1[], const Char str2[]);
	UInt utf8ToChar(const Char src[], UInt srcLength, Char dst[], UInt dstSize); // return dstLength
	UInt charToUTF8(const Char src[], UInt srcLength, Char dst[], UInt dstSize); // return dstLength
	UInt utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize); // return dstLength
	UInt wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize); // return dstLength
	UInt printToString(Char dst[], UInt dstSize, const Char format[], ...); // numeric only. return dstLength
	Bool scanFromString(const Char src[], UInt srcSize, const Char format[], Pointer val); // numeric only. return true if OK, only output one value
}
