
namespace spadas
{
	class StringVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(String, Vars)

		// buffer
		UInt size;
		WChar *data;
        
		// length
		UInt length;
		
		// for getter
		Char *bufferA;
		
		// constructors
		StringVars(UInt size0): size(size0), data(new WChar[size0]), length(0), bufferA(NULL)
		{
			data[0] = 0;
		}
        
		// destructor
        ~StringVars()
        {
            delete[] data;
            if (bufferA) delete[] bufferA;
        }
	};
}

namespace spadas_internal
{
	using namespace spadas;
	
	const UInt SIZE_LIMIT = 10000000;	// 10 millions

	Bool doubleToPrettyString(Double val, WChar output[48], Int& length);
	
	UInt lengthChar(const Char str[]);
	UInt lengthWChar(const WChar str[]);
	Int compareString(const WChar str1[], const WChar str2[]);
	UInt charToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize); // return dstLength
	UInt wCharToChar(const WChar src[], UInt srcLength, Char dst[], UInt dstSize); // return dstLength
	UInt utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize); // return dstLength
	UInt wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize); // return dstLength
	UInt printToString(WChar dst[], UInt dstSize, const WChar format[], ...); // return dstLength
	Bool scanFromString(const WChar src[], UInt srcSize, const WChar format[], Pointer val); // return true if OK, only output one value
}
