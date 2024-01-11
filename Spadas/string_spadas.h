
#include "spadas.h"

namespace spadas
{
	class StringVars : public Vars
	{
    public:
		SPADAS_VARS(String, Vars)

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
	Bool isSameString(const Byte *data1, const Byte *data2, UInt length);
	Int compareString(const Byte *data1, UInt length1, const Byte *data2, UInt length2);

	// Windows下的ANSI与WChar字符串互转
#if defined(SPADAS_ENV_WINDOWS)
	UInt ansiToWChar(const Byte src[], UInt srcLength, WChar dst[], UInt dstSize);
	UInt wCharToAnsi(const WChar src[], UInt srcLength, Byte dst[], UInt dstSize);
#endif

	class StringCommon
	{
	public:
		static Word getHashCode(const Byte* data, UInt len);
		static String clone(const Byte* bytes, UInt len);
		static Array<Char> chars(const Byte* bytes, UInt len);
		static Array<WChar> wchars(const Byte* bytes, UInt len);
		static StringAppender operatorPlus(const Byte* bytes, UInt originLength, String& append);
		static Optional<Int> toInt(const Byte* bytes, UInt len);
		static Bool toNumber(const Byte* bytes, UInt len, Int& number);
		static Optional<Long> toLong(const Byte* bytes, UInt len);
		static Bool toNumber(const Byte* bytes, UInt len, Long& number);
		static Optional<Float> toFloat(const Byte* bytes, UInt len);
		static Bool toNumber(const Byte* bytes, UInt len, Float& number);
		static Optional<Double> toDouble(const Byte* bytes, UInt len);
		static Bool toNumber(const Byte* bytes, UInt len, Double& number);
		static String toUpper(const Byte* bytes, UInt len);
		static String toLower(const Byte* bytes, UInt len);
		static Bool startsWith(const Byte* srcData, UInt srcLength, String& target);
		static Bool endsWith(const Byte* srcData, UInt srcLength, String& target);
		static Binary toBinary(const Byte* bytes, UInt len);
		static Array<UInt> search(const Byte* srcData, UInt srcLength, String& string);
		static UInt searchFirst(const Byte* srcData, UInt srcLength, String& string);
		static UInt searchLast(const Byte* srcData, UInt srcLength, String& string);
		static String trim(const Byte*srcData, UInt srcLength);
		static Array<StringSpan> split(const Byte* srcData, UInt srcLength, String& splitter, Vars *stringVars);
		static String replace(const Byte* srcStringData, UInt srcStringLength, String& oldString, String& newString);
	};
}
