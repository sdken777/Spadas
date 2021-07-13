
namespace spadas
{
	class StringVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(String, Vars)

		// buffer
		UInt size;
		Byte *data;
        
		// length
		UInt length;
		
		// constructors
		StringVars(UInt size0): size(size0), data(new Byte[size0]), length(0)
		{
			data[0] = 0;
		}
        
		// destructor
        ~StringVars()
        {
            delete[] data;
        }
	};
}

namespace spadas_internal
{
	using namespace spadas;
	
	const UInt SIZE_LIMIT = 10000000;	// 10 millions

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
