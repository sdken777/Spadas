
#include "spadas.h"

namespace spadas
{
	class StringVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(String, Vars)

		UInt size;
		UInt length;
		Byte* data;
		Byte* extData;
		
		// constructors
		StringVars(UInt size0, Byte* data0): size(size0), length(0), data(data0), extData(0)
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

namespace string_internal
{
	using namespace spadas;

	// 返回dst长度(不大于dstSize-1)，输出dst以0结尾
	UInt utf8ToWChar(const Char src[], UInt srcLength, WChar dst[], UInt dstSize);

	// 返回dst长度(不大于dstSize-1)，输出dst以0结尾
	UInt wCharToUTF8(const WChar src[], UInt srcLength, Char dst[], UInt dstSize);

	// 仅支持数值转换，返回dst长度(不大于dstSize-1)，输出dst以0结尾
	UInt printToString(Char dst[], UInt dstSize, const Char format[], ...);

	// 比较字符串
	Int compareString(const Byte *data1, UInt length1, const Byte *data2, UInt length2);

	// Windows下的ANSI与WChar字符串互转
#if defined(SPADAS_ENV_WINDOWS)
	UInt ansiToWChar(const Byte src[], UInt srcLength, WChar dst[], UInt dstSize);
	UInt wCharToAnsi(const WChar src[], UInt srcLength, Byte dst[], UInt dstSize);
#endif
}
