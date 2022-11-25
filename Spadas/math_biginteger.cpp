
#include "spadas.h"

////////////////////////////////////////////////////
// BigInt
#include <cassert>
#include <climits>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <iomanip>
#include <utility>
#include <limits>
#include <algorithm>
#include <stdexcept>

struct BigInt
{
	class Rossi;

	typedef unsigned long Ulong;

	static const std::size_t DEC_DIGIT = 10;
	static const std::size_t HEX_DIGIT = 16;
	static const std::size_t NIBBLE_BIT = 4;
	static const std::size_t ULONG_HEX_DIGITS = ((sizeof (Ulong)* CHAR_BIT) / NIBBLE_BIT);

	static const Ulong MAX_UNIT_VALUE = (ULONG_MAX >> 2);
	static const Ulong ULONG_MSB = static_cast<Ulong>(1) << (sizeof(Ulong)* CHAR_BIT - 1);

	static const Ulong BASE1 = 10;
	static const Ulong BASE2 = 1000000000;  // // BASE1 ** (BASE1 - 1)
	static const Ulong SUB_BASE2 = BASE2 - 1;  // 999999999
	static const Ulong OVER_BASE2 = 0xc0000000;  // OVER_BASE2 > SUB_BASE2

	template <typename T>
	static std::string toString(const T& t)
	{
		std::ostringstream oss;
		oss << t;
		return oss.str();
	}

	template<typename T, std::size_t N>
	static std::size_t getArrayElems(const T(&)[N])
	{
		return N;
	}

	template<typename T, std::size_t N>
	static std::size_t getArrayElems(T(&)[N])
	{
		return N;
	}

	template<typename T, std::size_t N>
	static std::size_t getArraySizeInBytes(const T(&a)[N])
	{
		return getArrayElems(a) * sizeof (T);
	}

	template<typename T, std::size_t N>
	static std::size_t getArraySizeInBytes(T(&a)[N])
	{
		return getArrayElems(a) * sizeof (T);
	}

	template<typename T, std::size_t N>
	static std::vector<T> array2vector(const T(&a)[N])
	{
		return std::vector<T>(a, a + getArrayElems(a));
	}

	template<typename T, std::size_t N>
	static std::vector<T> array2vector(T(&a)[N])
	{
		return std::vector<T>(a, a + getArrayElems(a));
	}

	template<typename T>
	static std::vector<T> elem2vector(const T& e)
	{
		return std::vector<T>(&e, &e + 1);
	}

	template<typename T>
	static std::vector<T> elem2vector(T& e)
	{
		return std::vector<T>(&e, &e + 1);
	}

	template<typename K, typename T, std::size_t N>
	static std::map<K, T> array2map(const std::pair<K, T>(&a)[N])
	{
		return std::map<K, T>(a, a + getArrayElems(a));
	}

	template<typename K, typename T, std::size_t N>
	static std::map<K, T> array2map(std::pair<K, T>(&a)[N])
	{
		return std::map<K, T>(a, a + getArrayElems(a));
	}

	template<typename K, typename T>
	static std::map<K, T> pair2map(const std::pair<K, T>& e)
	{
		return std::map<K, T>(&e, &e + 1);
	}

	class BaseBigInt
	{
	public:
		std::size_t getUnitsSize() const;
		void        maximize();

	protected:
		std::vector<Ulong>                  m_units;
		static std::map<char, std::size_t>  ms_hex2dec;

		void showUnits(std::ostream& o_stream) const;
		virtual ~BaseBigInt() = 0;

	public:
		bool  isEmpty() const;
	};

	class Vin : public BaseBigInt
	{
	private:
		friend std::ostream& operator<< (std::ostream& o_os, const Vin& i_ins);

	private:
		static Ulong s_carry;

		static Ulong    addUnits(Ulong n1, Ulong n2);
		bool            initViaString(const std::string& i_arg, std::size_t i_digitBase);

	public:
		explicit Vin();
		explicit Vin(Ulong i_unit);
		explicit Vin(const std::string& i_arg, std::size_t i_digitBase);

		std::string toStrDec(const std::string& i_prefix = "") const;

		Vin operator+ (const Vin& i_arg) const;
		Vin operator* (Ulong i_arg) const;
	};

	class Rossi : public BaseBigInt
	{
	private:
		friend std::ostream& operator<< (std::ostream& o_os, const Rossi& i_ins);

	private:
		void  resizeUnits(std::size_t i_size);
		void  truncateUnits();
		void  smartTruncateUnits();
		bool  backUnitIsNull() const;
		bool  initViaString(const std::string& i_arg, std::size_t i_digitBase);

	public:
		explicit Rossi();
		explicit Rossi(Ulong i_unit);
		explicit Rossi(const std::string& i_arg, std::size_t i_digitBase);
		explicit Rossi(const std::size_t i_noOfUnits, Ulong i_internalUnits, Ulong i_backUnit);

		Rossi  operator+ (const Rossi& i_arg);
		Rossi  operator+ (Ulong i_arg);
		Rossi  operator* (Rossi i_arg) const;
		Rossi  operator* (Ulong i_arg) const;
		Rossi  operator/ (const Rossi& i_arg) const;
		Rossi  operator% (const Rossi& i_arg) const;
		Rossi  divide(const Rossi& i_dividend, const Rossi& i_divisor, Rossi* o_remainder) const;
		Rossi& operator+= (const Rossi& i_arg);
		Rossi  operator++ (int); // Post increment operator
		Rossi& operator++ ();    // Pre increment operator
		Rossi  operator-  (const Rossi& i_arg);
		Rossi  operator-  ();
		Rossi  operator-- (int); // Post decrement operator
		Rossi& operator-- ();    // Pre decrement operator
		Rossi& operator-= (const Rossi& i_arg);
		Rossi  sqrt();

		Rossi  operator&   (const Rossi& i_arg);
		Rossi  operator|   (const Rossi& i_arg);
		Rossi  operator^   (const Rossi& i_arg);
		Rossi& operator&=  (const Rossi& i_arg);
		Rossi& operator|=  (const Rossi& i_arg);
		Rossi& operator^=  (const Rossi& i_arg);
		Rossi  operator~   ();
		Rossi  operator>>  (std::size_t i_shift);
		Rossi  operator<<  (std::size_t  i_shift);
		Rossi& operator>>= (std::size_t  i_shift);
		Rossi& operator<<= (std::size_t  i_shift);

		bool operator== (const Rossi& i_arg) const;
		bool operator!= (const Rossi& i_arg) const;
		bool operator<  (const Rossi& i_arg) const;
		bool operator>  (const Rossi& i_arg) const;
		bool operator<= (const Rossi& i_arg) const;
		bool operator>= (const Rossi& i_arg) const;

		std::string toStrHex(const std::string& i_prefix = "") const;
		std::string toStrDec(const std::string& i_prefix = "") const;
	};

}; // class BigInt

static const BigInt::Rossi RossiZero(0);
static const BigInt::Rossi RossiOne(1);
static const BigInt::Rossi RossiTwo(2);

static const std::pair<char, std::size_t> s_hex2dec[] =
{
	std::make_pair('0', 0),
	std::make_pair('1', 1),
	std::make_pair('2', 2),
	std::make_pair('3', 3),
	std::make_pair('4', 4),
	std::make_pair('5', 5),
	std::make_pair('6', 6),
	std::make_pair('7', 7),
	std::make_pair('8', 8),
	std::make_pair('9', 9),
	std::make_pair('a', 10),
	std::make_pair('A', 10),
	std::make_pair('b', 11),
	std::make_pair('B', 11),
	std::make_pair('c', 12),
	std::make_pair('C', 12),
	std::make_pair('d', 13),
	std::make_pair('D', 13),
	std::make_pair('e', 14),
	std::make_pair('E', 14),
	std::make_pair('f', 15),
	std::make_pair('F', 15)
};

std::map<char, std::size_t> BigInt::BaseBigInt::ms_hex2dec(array2map(s_hex2dec));

void BigInt::BaseBigInt::showUnits(std::ostream& o_stream) const
{
	std::ostringstream oss;

	const std::string::size_type widthSpaces = static_cast<std::string::size_type>(BigInt::toString(m_units.size()).size());

	oss << "--- Units: BEGIN ---"
		<< std::endl;
	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		oss << "m_units["
			<< i
			<< "]"
			<< std::string(widthSpaces, ' ')
			<< ": "
			<< std::hex
			<< std::showbase
			<< m_units[i]
			<< std::dec
			<< " = "
			<< m_units[i]
			<< std::endl;
	}
	oss << "---- Units: END ----"
		<< std::endl;


	o_stream << std::flush << oss.str() << std::flush;
}

bool BigInt::BaseBigInt::isEmpty() const
{
	return m_units.empty();
}

void BigInt::BaseBigInt::maximize()
{
	m_units.clear();

	const BigInt::Ulong maxUlong = std::numeric_limits<BigInt::Ulong>::max();

	while (true)
	{
		try
		{
			m_units.push_back(maxUlong);
		}
		catch (...)
		{
			// Do nothing
			break;
		}
	}
}

std::size_t BigInt::BaseBigInt::getUnitsSize() const
{
	return m_units.size();
}

BigInt::BaseBigInt::~BaseBigInt()
{
}

BigInt::Ulong BigInt::Vin::s_carry = 0;

BigInt::Ulong BigInt::Vin::addUnits(BigInt::Ulong n1, BigInt::Ulong n2)
{
	n1 += (n2 + s_carry);
	s_carry = (OVER_BASE2 & n1) || (n1 > SUB_BASE2);
	return (s_carry ? (n1 - BASE2) : n1);
}

BigInt::Vin::Vin()
{
}

BigInt::Vin::Vin(BigInt::Ulong i_unit)
{
	if (!(i_unit < BASE2))
	{
		return;
	}

	try
	{
		m_units.reserve(m_units.size() + 1);
	}
	catch (...)
	{
		return;
	}

	try
	{
		m_units.push_back(i_unit);
	}
	catch (...)
	{
		return;
	}
}

BigInt::Vin::Vin(const std::string& i_arg, std::size_t i_digitBase)
{
	initViaString(i_arg, i_digitBase);
}

bool BigInt::Vin::initViaString(const std::string& i_arg, std::size_t i_digitBase)
{
	m_units.push_back(0);

	for (std::size_t i = 0; i < i_arg.size(); i++)
	{
		switch (i_digitBase)
		{
		case DEC_DIGIT:
			if (!isdigit(i_arg[i]))
			{
				return false; // unused
			}
			break;

		case HEX_DIGIT:
			if (!isxdigit(i_arg[i]))
			{
				return false; // unused
			}
			break;

		default:
			return false;
			break; // unused
		}  // switch (i_digitBase)
	} // for (std::size_t i = 0; i < i_arg.size(); i++)

	for (std::size_t i = 0; i < i_arg.size(); i++)
	{
		*this = (*this) * static_cast<BigInt::Ulong>(i_digitBase)
			+
			BigInt::Vin(static_cast<BigInt::Ulong>(ms_hex2dec[i_arg[i]]));
	}
	return true;
}

BigInt::Vin BigInt::Vin::operator+ (const BigInt::Vin& i_arg) const
{
	BigInt::Vin ret;
	const std::size_t maxSize(std::max(m_units.size(), i_arg.m_units.size()));

	std::vector<BigInt::Ulong> u1(m_units);
	std::vector<BigInt::Ulong> u2(i_arg.m_units);

	try
	{
		u1.reserve(maxSize);
	}
	catch (...)
	{
		return ret;
	}

	try
	{
		u2.reserve(maxSize);
	}
	catch (...)
	{
		return ret;
	}

	try
	{
		ret.m_units.reserve(maxSize + 1);
	}
	catch (...)
	{
		return ret;
	}

	try
	{
		u1.resize(maxSize);
	}
	catch (...)
	{
		return ret;
	}

	try
	{
		u2.resize(maxSize);
	}
	catch (...)
	{
		return ret;
	}

	try
	{
		ret.m_units.resize(maxSize);
	}
	catch (...)
	{
		return ret;
	}

	s_carry = 0;
	std::transform(&u1[0], &u1[0] + maxSize, &u2[0], &ret.m_units[0], BigInt::Vin::addUnits);

	if (s_carry)
	{
		try
		{
			ret.m_units.push_back(s_carry);
		}
		catch (...)
		{
			return ret;
		}
	}

	return ret;
}

BigInt::Vin BigInt::Vin::operator* (BigInt::Ulong i_arg) const
{
	BigInt::Vin ret(0);
	for (std::size_t i = 0; i < i_arg; i++)
	{
		ret = ret + (*this);
	}
	return ret;
}

std::string BigInt::Vin::toStrDec(const std::string& i_prefix) const
{
	std::ostringstream oss;

	if (isEmpty())
	{
		return "isEmpty";
	}

	oss << i_prefix;

	for (std::size_t i = (m_units.size() - 1); i; --i)
	{
		oss << m_units[i]
			<< std::setw(BigInt::BASE1 - 1)
			<< std::setfill('0');
	}
	oss << m_units[0];

	return oss.str();
}

std::ostream& operator<< (std::ostream& o_os, const BigInt::Vin& i_ins)
{
	return o_os << i_ins.toStrDec();
}

BigInt::Rossi::Rossi()
{
}

BigInt::Rossi::Rossi(BigInt::Ulong i_unit)
{
	m_units.push_back(i_unit);
}

BigInt::Rossi::Rossi(const std::string& i_arg, std::size_t i_digitBase)
{
	initViaString(i_arg, i_digitBase);
}

BigInt::Rossi::Rossi(const std::size_t i_noOfUnits, BigInt::Ulong i_internalUnits, BigInt::Ulong i_backUnit)
{
	try
	{
		m_units.resize(i_noOfUnits, i_internalUnits);
	}
	catch (...)
	{
		return;
	}

	m_units.back() = i_backUnit;
}

bool BigInt::Rossi::initViaString(const std::string& i_arg, std::size_t i_digitBase)
{
	m_units.push_back(0);

	for (std::size_t i = 0; i < i_arg.size(); i++)
	{
		switch (i_digitBase)
		{
		case DEC_DIGIT:
			if (!isdigit(i_arg[i]))
			{
				return false; // unused
			}
			break;

		case HEX_DIGIT:
			if (!isxdigit(i_arg[i]))
			{
				std::ostringstream ossErr;
				ossErr << ""
					<< "std::string contains non-hexadecimal digit"
					<< std::endl
					<< "std::string = "
					<< i_arg
					<< std::endl
					<< i
					<< "-th char = "
					<< i_arg[i]
					<< std::endl
					<< std::endl;

				return false; // unused
			}
			break;

		default:
			return false;
			break; // unused
		}
	}

	for (std::size_t i = 0; i < i_arg.size(); i++)
	{
		*this = (*this) * static_cast<BigInt::Ulong>(i_digitBase)
			+
			BigInt::Rossi(static_cast<BigInt::Ulong>(ms_hex2dec[i_arg[i]]));
	}
	return true;
}

void BigInt::Rossi::resizeUnits(std::size_t i_size)
{
	m_units.resize(i_size);
}

void BigInt::Rossi::truncateUnits()
{
	while ((m_units.size() > 1) && (m_units.back() == 0))
	{
		m_units.pop_back();
	}
}

void BigInt::Rossi::smartTruncateUnits()
{
	if (backUnitIsNull())
	{
		truncateUnits();
	}
}

bool BigInt::Rossi::backUnitIsNull() const
{
	if (m_units.size() == 1)
	{
		return false;
	}
	return (m_units.back() == 0);
}

BigInt::Rossi BigInt::Rossi::operator+ (const BigInt::Rossi& i_arg)
{
	BigInt::Rossi ret(0);
	BigInt::Rossi arg(i_arg);
	BigInt::Ulong carry = 0;

	const std::size_t maxSize(std::max(getUnitsSize(), arg.getUnitsSize()));

	resizeUnits(maxSize + 1);
	arg.resizeUnits(maxSize + 1);
	ret.resizeUnits(maxSize + 1);

	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		ret.m_units[i] = m_units[i] + arg.m_units[i] + carry;
		if (carry == 0)
		{
			carry = ((ret.m_units[i] < m_units[i] || ret.m_units[i] < arg.m_units[i]) ? 1 : 0);
		}
		else
		{
			carry = ((ret.m_units[i] <= m_units[i] || ret.m_units[i] <= arg.m_units[i]) ? 1 : 0);
		}
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator+ (BigInt::Ulong i_arg)
{
	return (*this + BigInt::Rossi(i_arg));
}

bool BigInt::Rossi::operator< (const BigInt::Rossi& i_arg) const
{
	if (m_units.size() < i_arg.m_units.size())
	{
		return true;
	}

	if (i_arg.m_units.size() < m_units.size())
	{
		return false;
	}

	for (std::size_t i = (m_units.size() - 1); i > 0; i--)
	{
		if (m_units[i] < i_arg.m_units[i])
		{
			return true;
		}

		if (i_arg.m_units[i] < m_units[i])
		{
			return false;
		}
	}
	return (m_units[0] < i_arg.m_units[0]);
}

bool BigInt::Rossi::operator<= (const BigInt::Rossi& i_arg) const
{
	if (*this < i_arg)
	{
		return true;
	}

	if (*this == i_arg)
	{
		return true;
	}

	return false;
}

bool BigInt::Rossi::operator> (const BigInt::Rossi& i_arg) const
{
	return (!(*this <= i_arg));
}

bool BigInt::Rossi::operator>= (const BigInt::Rossi& i_arg) const
{
	return (!(*this < i_arg));
}

bool BigInt::Rossi::operator== (const BigInt::Rossi& i_arg) const
{
	if (*this < i_arg)
	{
		return false;
	}

	if (i_arg < *this)
	{
		return false;
	}

	return true;
}

bool BigInt::Rossi::operator!= (const BigInt::Rossi& i_arg) const
{
	return (!(*this == i_arg));
}

BigInt::Rossi BigInt::Rossi::operator/ (const BigInt::Rossi& i_arg) const
{
	return divide(*this, i_arg, NULL);
}

BigInt::Rossi BigInt::Rossi::operator% (const BigInt::Rossi& i_arg) const
{
	BigInt::Rossi ret;
	divide(*this, i_arg, &ret);
	return ret;
}

BigInt::Rossi BigInt::Rossi::operator>> (std::size_t i_shift)
{
	BigInt::Rossi tmp;
	BigInt::Rossi ret;

	tmp = *this;

	ret.resizeUnits(m_units.size());

	for (std::size_t i = 0; i < i_shift; i++)
	{
		ret.m_units.back() = (tmp.m_units.back() >> 1);

		for (std::size_t j1 = tmp.m_units.size(); j1; j1--)
		{
			const std::size_t j = j1 - 1;
			ret.m_units[j] = (tmp.m_units[j] >> 1);

			if ((tmp.m_units[j + 1] & 1) != 0)
			{
				ret.m_units[j] |= ULONG_MSB;      // Set MSB bit
			}
		}
		tmp = ret;
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator<< (std::size_t i_shift)
{
	BigInt::Rossi tmp;
	BigInt::Rossi ret;

	tmp = *this;

	ret.resizeUnits(m_units.size() + 1);

	for (std::size_t i = 0; i < i_shift; i++)
	{
		ret.m_units.front() = (tmp.m_units.front() << 1);
		for (std::size_t j = 1; j < tmp.m_units.size(); j++)
		{
			ret.m_units[j] = (tmp.m_units[j] << 1);
			if ((tmp.m_units[j - 1] & ULONG_MSB) != 0)
			{
				ret.m_units[j] |= 1;     // Set MSB bit
			}
		}
		if ((tmp.m_units.back() & ULONG_MSB) != 0)
		{
			ret.m_units.back() |= 1;   // Set MSB bit
		}
		tmp = ret;
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi& BigInt::Rossi::operator>>= (std::size_t i_shift)
{
	BigInt::Ulong carry;
	m_units.push_back(0);

	for (std::size_t i = 0; i < i_shift; i++)
	{
		carry = m_units.back() & 1;
		m_units.back() >>= 1;

		for (std::size_t j1 = m_units.size(); j1; j1--)
		{
			const std::size_t j = j1 - 1;
			if (carry)
			{
				carry = m_units[j] & 1;
				m_units[j] >>= 1;
				m_units[j] |= ULONG_MSB;
			}
			else
			{
				carry = m_units[j] & 1;
				m_units[j] >>= 1;
			}
		}
	}

	smartTruncateUnits();

	return *this;
}

BigInt::Rossi& BigInt::Rossi::operator<<= (std::size_t i_shift)
{
	BigInt::Ulong carry;

	const std::size_t pushBackSize(i_shift / (sizeof (std::size_t) * CHAR_BIT) + 1);

	for (std::size_t i = 0; i < (pushBackSize + 1); i++)
	{
		try
		{
			m_units.push_back(0);
		}
		catch (...)
		{
			return *this;
		}
	}

	for (std::size_t i = 0; i < i_shift; i++)
	{
		carry = m_units.front() & ULONG_MSB;
		m_units.front() <<= 1;

		for (std::size_t j = 1; j < m_units.size(); j++)
		{
			if (carry)
			{
				carry = m_units[j] & ULONG_MSB;
				m_units[j] <<= 1;
				m_units[j] |= 1;
			}
			else
			{
				carry = m_units[j] & ULONG_MSB;
				m_units[j] <<= 1;
			}
		}
	}

	smartTruncateUnits();

	return *this;
}

BigInt::Rossi& BigInt::Rossi::operator+=(const BigInt::Rossi& i_arg)
{
	BigInt::Ulong carry = 0;
	BigInt::Ulong prevDigit;
	BigInt::Rossi arg(i_arg);

	const std::size_t maxSize(std::max(getUnitsSize(), arg.getUnitsSize()));

	resizeUnits(maxSize + 1);
	arg.resizeUnits(maxSize + 1);

	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		prevDigit = m_units[i];
		m_units[i] = m_units[i] + arg.m_units[i] + carry;
		if (carry == 0)
		{
			carry = ((m_units[i] < prevDigit || m_units[i] < arg.m_units[i]) ? 1 : 0);
		}
		else
		{
			carry = ((m_units[i] <= prevDigit || m_units[i] <= arg.m_units[i]) ? 1 : 0);
		}
	}

	smartTruncateUnits();
	return *this;
}

BigInt::Rossi& BigInt::Rossi::operator++()  // Pre Increment operator -- faster than add
{
	m_units.push_back(0);

	m_units.front()++;
	for (std::size_t i = 1; i < m_units.size(); i++)
	{
		if (m_units[i - 1])
		{
			break;
		}
		m_units[i]++;
	}

	smartTruncateUnits();

	return *this;
}

BigInt::Rossi BigInt::Rossi::operator++ (int)  // Post Increment operator -- faster than add
{
	BigInt::Rossi tmp(*this);
	++*this;
	return tmp;
}

BigInt::Rossi BigInt::Rossi::operator- ()  // Negates a number
{
	BigInt::Rossi ret;
	ret.resizeUnits(m_units.size() + 1);

	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		ret.m_units[i] = ~m_units[i];
	}

	ret = ret + RossiOne;

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator-(const BigInt::Rossi& i_arg)
{
	BigInt::Rossi ret(RossiZero);
	BigInt::Rossi arg(i_arg);
	BigInt::Ulong borrow = 0;

	const std::size_t maxSize(std::max(getUnitsSize(), arg.getUnitsSize()));

	resizeUnits(maxSize + 1);
	arg.resizeUnits(maxSize + 1);
	ret.resizeUnits(maxSize + 1);

	if (*this < arg)
	{
		return ret;
	}

	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		ret.m_units[i] = m_units[i] - arg.m_units[i] - borrow;

		if (borrow == 0)
		{
			borrow = ((m_units[i] < arg.m_units[i]) ? 1 : 0);
		}
		else
		{
			borrow = ((m_units[i] <= arg.m_units[i]) ? 1 : 0);
		}
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi& BigInt::Rossi::operator-= (const BigInt::Rossi& i_arg)
{
	BigInt::Ulong borrow = 0;
	BigInt::Rossi arg(i_arg);
	BigInt::Ulong prevDigit;

	const std::size_t maxSize(std::max(getUnitsSize(), arg.getUnitsSize()));
	resizeUnits(maxSize + 1);
	arg.resizeUnits(maxSize + 1);

	if (*this < arg)
	{
		return *this;
	}

	for (std::size_t i = 0; i < m_units.size(); i++)
	{
		prevDigit = m_units[i];
		m_units[i] = m_units[i] - arg.m_units[i] - borrow;

		if (borrow == 0)
		{
			borrow = ((prevDigit < arg.m_units[i]) ? 1 : 0);
		}
		else
		{
			borrow = ((prevDigit <= arg.m_units[i]) ? 1 : 0);
		}
	}

	smartTruncateUnits();

	return *this;
}

BigInt::Rossi& BigInt::Rossi::operator--()  // Pre Decrement operator -- faster than add
{
	m_units.front()--;
	for (std::size_t i = 1; i < m_units.size(); i++)
	{
		if (m_units[i - 1] != ULONG_MAX)
		{
			break;
		}

		m_units[i]--;
	}

	smartTruncateUnits();
	return *this;
}

BigInt::Rossi BigInt::Rossi::operator-- (int)  // Post Decrement operator -- faster than add
{
	BigInt::Rossi tmp(*this);
	--*this;
	return tmp;
}

BigInt::Rossi BigInt::Rossi::operator& (const BigInt::Rossi& i_arg)
{
	const std::size_t maxSize(std::max(getUnitsSize(), i_arg.getUnitsSize()));

	BigInt::Rossi ret;
	BigInt::Rossi arg(i_arg);

	ret.resizeUnits(maxSize);
	arg.resizeUnits(maxSize);
	resizeUnits(maxSize);

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		ret.m_units[i] = m_units[i] & arg.m_units[i];
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator| (const BigInt::Rossi& i_arg)
{
	const std::size_t maxSize(std::max(getUnitsSize(), i_arg.getUnitsSize()));

	BigInt::Rossi ret;
	BigInt::Rossi arg(i_arg);

	ret.resizeUnits(maxSize);
	arg.resizeUnits(maxSize);
	resizeUnits(maxSize);

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		ret.m_units[i] = m_units[i] | arg.m_units[i];
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator^ (const BigInt::Rossi& i_arg)
{
	const std::size_t maxSize(std::max(getUnitsSize(), i_arg.getUnitsSize()));

	BigInt::Rossi ret;
	BigInt::Rossi arg(i_arg);

	ret.resizeUnits(maxSize);
	arg.resizeUnits(maxSize);
	resizeUnits(maxSize);

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		ret.m_units[i] = m_units[i] ^ arg.m_units[i];
	}


	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator~ ()
{
	BigInt::Rossi ret;

	ret.resizeUnits(getUnitsSize());

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		ret.m_units[i] = ~m_units[i];
	}

	smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi& BigInt::Rossi::operator&= (const BigInt::Rossi& i_arg)
{
	const std::size_t maxSize(std::max(getUnitsSize(), i_arg.getUnitsSize()));

	BigInt::Rossi arg(i_arg);

	arg.resizeUnits(maxSize);
	resizeUnits(maxSize);

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		m_units[i] = m_units[i] & arg.m_units[i];
	}

	smartTruncateUnits();

	return *this;

}

BigInt::Rossi& BigInt::Rossi::operator|=(const BigInt::Rossi& i_arg)
{
	const std::size_t maxSize(std::max(getUnitsSize(), i_arg.getUnitsSize()));

	BigInt::Rossi arg(i_arg);

	arg.resizeUnits(maxSize);
	resizeUnits(maxSize);

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		m_units[i] = m_units[i] | arg.m_units[i];
	}

	smartTruncateUnits();

	return *this;
}

BigInt::Rossi& BigInt::Rossi::operator^=(const BigInt::Rossi& i_arg)
{
	const std::size_t maxSize(std::max(getUnitsSize(), i_arg.getUnitsSize()));

	BigInt::Rossi arg(i_arg);

	arg.resizeUnits(maxSize);
	resizeUnits(maxSize);

	for (std::size_t i1 = m_units.size(); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		m_units[i] = m_units[i] ^ arg.m_units[i];
	}

	smartTruncateUnits();
	return *this;
}

BigInt::Rossi BigInt::Rossi::operator* (BigInt::Rossi i_arg) const
{
	BigInt::Rossi tmp;
	BigInt::Rossi ret;

	tmp = *this;
	ret = RossiZero;

	do
	{
		if ((i_arg.m_units.front() & 1) != 0)
		{
			ret += tmp;
		}
		i_arg >>= 1;
		tmp <<= 1;
	} while (i_arg != RossiZero);

	const_cast<BigInt::Rossi*>(this)->smartTruncateUnits();
	ret.smartTruncateUnits();

	return ret;
}

BigInt::Rossi BigInt::Rossi::operator* (BigInt::Ulong i_arg) const
{
	return ((*this) * BigInt::Rossi(i_arg));
}

BigInt::Rossi BigInt::Rossi::divide(
	const BigInt::Rossi& i_dividend,
	const BigInt::Rossi& i_divisor,
	BigInt::Rossi*       o_remainder
	) const
{
	BigInt::Rossi dividend(i_dividend);
	BigInt::Rossi divisor(i_divisor);

	long shiftcnt(0);

	// --- Check for attempt to divide by zero ---
	if (divisor == RossiZero)
	{
		return RossiZero;
	}

	BigInt::Rossi quotient(RossiZero);

	quotient.resizeUnits(dividend.getUnitsSize());

	if (o_remainder != NULL)
	{
		o_remainder->resizeUnits(dividend.getUnitsSize());
	}

	// --- Left shift divisor until it is greater than or equal to dividend ---
	// while ( divisor < dividend && ((divisor.m_units.back() & ULONG_MSB) == 0))
	while (divisor < dividend)
	{
		divisor <<= 1;
		shiftcnt++;
	}

	if (divisor > dividend)      // If divisor is greater than dividend, right shift divisor
	{
		divisor >>= 1;
		shiftcnt--;
	}

	if (shiftcnt >= 0)
	{
		for (long i = 0; i <= shiftcnt; i++)
		{
			if (divisor <= dividend)  // If divisor is greater than dividend, then the bit is a 1 
			{
				dividend -= divisor;     // Subtract divisor from dividend 
				divisor >>= 1;          // Right shift divisor 
				quotient <<= 1;          // Left shift quotient
				quotient++;              // Increment quotient           // Increment quotient 
			}
			else
			{
				divisor >>= 1;             // Bit is 0, right shift divisor, left shift quotient
				quotient <<= 1;
			}
		}
	}

	BigInt::Rossi remainder(dividend);
	remainder.smartTruncateUnits();

	if (o_remainder != NULL)
	{
		*o_remainder = remainder;
		o_remainder->smartTruncateUnits();
	}

	quotient.smartTruncateUnits();

	return quotient;
}

BigInt::Rossi BigInt::Rossi::sqrt()		// Returns the square root of this
{
	BigInt::Rossi ret;
	BigInt::Rossi tmp;
	BigInt::Rossi delta;

	BigInt::Rossi mask(RossiTwo);

	tmp = *this;
	mask = -mask;

	std::size_t i = 0;
	ret = tmp;
	for (i = 0; ret != RossiZero; ret >>= 1, i++)
	{
		// Do nothing
	}

	ret = tmp;
	for (std::size_t j = 0; j < std::size_t(i >> 1); ret >>= 1, j++)
	{
		// Do nothing
	}

	do
	{
		// -----------------------------------------------
		// We are really performing the fuction:
		//	 delta = (tmp/ret - ret) / 2;
		//   below, but since these are unsigned numbers,
		//   we MUST do the subtraction last in order for
		//   the ret = ret + delta;  equation to work properly.
		// -----------------------------------------------

		delta = (tmp >> BigInt::Ulong(1)) / ret - (ret >> BigInt::Ulong(1));
		ret = ret + delta;
	} while ((delta &= mask) != RossiZero);

	return ret;
}

std::string BigInt::Rossi::toStrHex(const std::string& i_prefix) const
{
	const std::size_t HEX_SETW = sizeof(BigInt::Ulong) * 2;
	std::ostringstream oss;

	if (m_units.empty())
	{
		oss << i_prefix
			<< std::hex
			<< static_cast<BigInt::Ulong>(0)
			<< std::dec;

		return oss.str();
	}

	// --------------
	oss << i_prefix
		<< std::hex
		<< m_units.back();

	for (std::size_t i1 = (m_units.size() - 1); i1; i1--)
	{
		const std::size_t i = i1 - 1;
		oss << std::setw(HEX_SETW)
			<< std::setfill('0')
			<< m_units[i];
	}
	oss << std::dec;
	return oss.str();
}

std::string BigInt::Rossi::toStrDec(const std::string& i_prefix) const
{
	std::ostringstream oss;

	BigInt::Vin vin(toStrHex(), BigInt::HEX_DIGIT);

	oss << vin;

	return oss.str();
}

////////////////////////////////////////////////////
// BigInteger
namespace spadas
{
	class BigIntegerVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(BigInteger, Vars)

		Bool sign; // TRUE: positive or zero, FALSE: negative
		BigInt::Rossi rossi;

		String toString() override
		{
			if (sign) return rossi.toStrDec().data();
			else return (String)"-" + rossi.toStrDec().data();
		}
	};
}

using namespace spadas;

const String spadas::BigInteger::TypeName = "spadas.BigInteger";

BigInteger::BigInteger() : Object<BigIntegerVars>(new BigIntegerVars, TRUE)
{
	vars->rossi = BigInt::Rossi("0", 10);
	vars->sign = TRUE;
}

BigInteger::BigInteger(Binary integerData, Bool sign) : Object<BigIntegerVars>(new BigIntegerVars, TRUE)
{
	UInt validIndex = UINF;
	for (UInt i = 0; i < integerData.size(); i++)
	{
		if (integerData[i] != 0)
		{
			validIndex = i;
			break;
		}
	}

	if (validIndex == UINF)
	{
		vars->rossi = BigInt::Rossi("0", 10);
		vars->sign = TRUE;
		return;
	}

	Binary hexText((integerData.size() - validIndex) * 2 + 1);
	Byte* hexTextData = hexText.data();
	for (UInt i = validIndex; i < integerData.size(); i++)
	{
		Byte byteVal = integerData[i];
		Int upper = byteVal / 16, lower = byteVal % 16;
		hexTextData[2 * (i - validIndex)] = (upper >= 10) ? ('A' + (upper - 10)) : ('0' + upper);
		hexTextData[2 * (i - validIndex) + 1] = (lower >= 10) ? ('A' + (lower - 10)) : ('0' + lower);
	}
	hexTextData[(integerData.size() - validIndex) * 2] = 0;

	vars->rossi = BigInt::Rossi((Char*)hexTextData, 16);
	vars->sign = vars->rossi == BigInt::Rossi() ? TRUE : sign;
}

Optional<BigInteger> BigInteger::createFromString(String decimalString)
{
	if (decimalString.isEmpty() || decimalString == "-0") decimalString = "0";

	Byte* stringData = decimalString.bytes();
	UInt stringLength = decimalString.length();
	Bool isNegative = stringData[0] == (Byte)'-';

	for (UInt i = isNegative ? 1 : 0; i < stringLength; i++)
	{
		if (stringData[i] < (Byte)'0' || stringData[i] > (Byte)'9') return Optional<BigInteger>();
	}

	BigInteger ret;
	ret.vars->sign = !isNegative;
	ret.vars->rossi = BigInt::Rossi((Char*)&stringData[isNegative ? 1 : 0], 10);
	return ret;
}

Bool BigInteger::isPositive()
{
	return vars->sign;
}

Binary BigInteger::getData()
{
	String hexText = vars->rossi.toStrHex().data();
	if (hexText == "0")
	{
		Binary ret(1);
		ret[0] = 0;
		return ret;
	}

	Binary ret((hexText.length() + 1) / 2);
	
	Byte* hexTextData = hexText.bytes();
	UInt retStartIndex = 0, hexTextStartIndex = 0;
	if (hexText.length() % 2 == 1)
	{
		Byte hexChar = hexTextData[0];
		ret[0] = hexChar > '9' ? (hexChar - 'a' + 10) : (hexChar - '0');
		retStartIndex = hexTextStartIndex = 1;
	}

	for (UInt i = retStartIndex, n = hexTextStartIndex; i < ret.size(); i++, n += 2)
	{
		Byte hexChar1 = hexTextData[n];
		Byte hexChar2 = hexTextData[n + 1];
		ret[i] = ((hexChar1 > '9' ? (hexChar1 - 'a' + 10) : (hexChar1 - '0')) << 4) +
			(hexChar2 > '9' ? (hexChar2 - 'a' + 10) : (hexChar2 - '0'));
	}

	return ret;
}

String BigInteger::toString()
{
	if (vars->sign) return vars->rossi.toStrDec().data();
	else return (String)"-" + vars->rossi.toStrDec().data();
}

Word BigInteger::getHash()
{
	return toString().getHash();
}

Bool BigInteger::operator ==(BigInteger num)
{
	return vars->sign == num.vars->sign && vars->rossi == num.vars->rossi;
}
Bool BigInteger::operator !=(BigInteger num)
{
	return vars->sign != num.vars->sign || vars->rossi != num.vars->rossi;
}
Bool BigInteger::operator <(BigInteger num)
{
	if (vars->sign == TRUE && num.vars->sign == FALSE) return FALSE;
	else if (vars->sign == FALSE && num.vars->sign == TRUE) return TRUE;
	else if (vars->sign == TRUE) return vars->rossi < num.vars->rossi;
	else return vars->rossi > num.vars->rossi;
}
Bool BigInteger::operator >(BigInteger num)
{
	if (vars->sign == TRUE && num.vars->sign == FALSE) return TRUE;
	else if (vars->sign == FALSE && num.vars->sign == TRUE) return FALSE;
	else if (vars->sign == TRUE) return vars->rossi > num.vars->rossi;
	else return vars->rossi < num.vars->rossi;
}

BigInteger BigInteger::operator +(BigInteger num)
{
	BigInteger ret;
	if (vars->sign == num.vars->sign)
	{
		ret.vars->sign = vars->sign;
		ret.vars->rossi = vars->rossi + num.vars->rossi;
	}
	else
	{
		if (vars->rossi == num.vars->rossi) return ret;
		if (vars->rossi > num.vars->rossi)
		{
			ret.vars->sign = vars->sign;
			ret.vars->rossi = vars->rossi - num.vars->rossi;
		}
		else
		{
			ret.vars->sign = num.vars->sign;
			ret.vars->rossi = num.vars->rossi - vars->rossi;
		}
	}
	return ret;
}
BigInteger BigInteger::operator -(BigInteger num)
{
	BigInteger ret;
	if (vars->sign != num.vars->sign)
	{
		ret.vars->sign = vars->sign;
		ret.vars->rossi = vars->rossi + num.vars->rossi;
	}
	else
	{
		if (vars->rossi == num.vars->rossi) return ret;
		if (vars->rossi > num.vars->rossi)
		{
			ret.vars->sign = vars->sign;
			ret.vars->rossi = vars->rossi - num.vars->rossi;
		}
		else
		{
			ret.vars->sign = !num.vars->sign;
			ret.vars->rossi = num.vars->rossi - vars->rossi;
		}
	}
	return ret;
}
BigInteger BigInteger::operator *(BigInteger num)
{
	BigInteger ret;
	if (vars->rossi == ret.vars->rossi || num.vars->rossi == ret.vars->rossi) return ret;
	ret.vars->sign = vars->sign ? num.vars->sign : !num.vars->sign;
	ret.vars->rossi = vars->rossi * num.vars->rossi;
	return ret;
}
BigInteger BigInteger::operator /(BigInteger num)
{
	BigInteger ret;
	SPADAS_ERROR_RETURNVAL(num.vars->rossi == ret.vars->rossi, ret);
	ret.vars->sign = vars->sign ? num.vars->sign : !num.vars->sign;
	ret.vars->rossi = vars->rossi / num.vars->rossi;
	return ret;
}
BigInteger BigInteger::operator %(BigInteger num)
{
	BigInteger ret;
	SPADAS_ERROR_RETURNVAL(num.vars->rossi == ret.vars->rossi, ret);
	ret.vars->sign = vars->sign;
	ret.vars->rossi = vars->rossi % num.vars->rossi;
	return ret;
}

BigInteger BigInteger::modPow(BigInteger exponent, BigInteger modulus)
{
	BigInteger zero;
	SPADAS_ERROR_RETURNVAL(exponent.vars->rossi == zero.vars->rossi || modulus.vars->rossi == zero.vars->rossi, zero);
	SPADAS_ERROR_RETURNVAL(!modulus.isPositive(), zero);

	if (vars->rossi == zero.vars->rossi) return zero;
	SPADAS_ERROR_RETURNVAL(!vars->sign, zero);

	BigInteger one = BigInteger::createFromString("1").value();
	BigInteger res = BigInteger::createFromString("1").value();
	BigInt::Rossi a = exponent.vars->rossi;
	for (BigInteger p = (*this); a != zero.vars->rossi; a >>= 1, p = (p * p) % modulus)
	{
		if ((a & one.vars->rossi) != zero.vars->rossi) res = (res * p) % modulus;
	}
	return res;
}
