
#include "3party/BigIntegerWrapper.h"
#include "spadas.h"

namespace spadas
{
	class BigIntegerVars : public Vars
	{
	public:
		SPADAS_VARS(BigInteger, Vars)

		Bool sign; // TRUE: positive or zero, FALSE: negative
		BigIntegerWrapper bigIntNum;

		String toString() override
		{
			String numString(bigIntNum.ToString().c_str());
			if (sign) return numString;
			else return (String)"-" + numString;
		}
	};
}

using namespace spadas;

BigInteger::BigInteger() : BaseObject(new BigIntegerVars)
{
	var()->bigIntNum = std::string("0");
	var()->sign = TRUE;
}

BigInteger::BigInteger(Binary integerData, Bool sign) : BaseObject(new BigIntegerVars)
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
		var()->bigIntNum = std::string("0");
		var()->sign = TRUE;
		return;
	}

	std::vector<uint8_t> vector(integerData.size() - validIndex);
	for (UInt i = validIndex; i < integerData.size(); i++) vector.push_back(integerData[i]);
	var()->bigIntNum = vector;

	var()->sign = var()->bigIntNum.Equals(std::string("0")) ? TRUE : sign;
}

Optional<BigInteger> BigInteger::createFromString(String decimalString)
{
	if (decimalString.isEmpty() || decimalString == "-" || decimalString == "-0") decimalString = "0";

	Array<Char> chars = decimalString.chars();
	Char* stringData = chars.data();
	UInt stringLength = decimalString.length();
	Bool isNegative = stringData[0] == '-';

	for (UInt i = isNegative ? 1 : 0; i < stringLength; i++)
	{
		if (stringData[i] < '0' || stringData[i] > '9') return Optional<BigInteger>();
	}

	BigInteger ret;
	ret.var()->sign = !isNegative;
	ret.var()->bigIntNum = std::string((Char*)&stringData[isNegative ? 1 : 0]);
	return ret;
}

Bool BigInteger::isPositive()
{
	return var()->sign;
}

Binary BigInteger::getData()
{
	std::vector<uint8_t> vector = var()->bigIntNum.ToByteArrayUnsigned();

	Binary output(vector.size());
	for (UInt i = 0; i < output.size(); i++) output[i] = vector[i];
	return output;
}

Word BigInteger::getHash()
{
	return var()->toString().getHash();
}

Bool BigInteger::operator ==(BigInteger num)
{
	return var()->sign == num.var()->sign && var()->bigIntNum.Equals(num.var()->bigIntNum);
}
Bool BigInteger::operator !=(BigInteger num)
{
	return var()->sign != num.var()->sign || !var()->bigIntNum.Equals(num.var()->bigIntNum);
}
Bool BigInteger::operator <(BigInteger num)
{
	if (var()->sign == TRUE && num.var()->sign == FALSE) return FALSE;
	else if (var()->sign == FALSE && num.var()->sign == TRUE) return TRUE;
	else if (var()->sign == TRUE) return var()->bigIntNum.CompareTo(num.var()->bigIntNum) < 0;
	else return var()->bigIntNum.CompareTo(num.var()->bigIntNum) > 0;
}
Bool BigInteger::operator >(BigInteger num)
{
	if (var()->sign == TRUE && num.var()->sign == FALSE) return TRUE;
	else if (var()->sign == FALSE && num.var()->sign == TRUE) return FALSE;
	else if (var()->sign == TRUE) return var()->bigIntNum.CompareTo(num.var()->bigIntNum) > 0;
	else return var()->bigIntNum.CompareTo(num.var()->bigIntNum) < 0;
}

BigInteger BigInteger::operator +(BigInteger num)
{
	BigInteger ret;
	if (var()->sign == num.var()->sign)
	{
		ret.var()->sign = var()->sign;
		ret.var()->bigIntNum = var()->bigIntNum.Add(num.var()->bigIntNum);
	}
	else
	{
		if (var()->bigIntNum.Equals(num.var()->bigIntNum)) return ret;
		if (var()->bigIntNum.CompareTo(num.var()->bigIntNum) > 0)
		{
			ret.var()->sign = var()->sign;
			ret.var()->bigIntNum = var()->bigIntNum.Subtract(num.var()->bigIntNum);
		}
		else
		{
			ret.var()->sign = num.var()->sign;
			ret.var()->bigIntNum = num.var()->bigIntNum.Subtract(var()->bigIntNum);
		}
	}
	return ret;
}
BigInteger BigInteger::operator -(BigInteger num)
{
	BigInteger ret;
	if (var()->sign != num.var()->sign)
	{
		ret.var()->sign = var()->sign;
		ret.var()->bigIntNum = var()->bigIntNum.Add(num.var()->bigIntNum);
	}
	else
	{
		if (var()->bigIntNum.Equals(num.var()->bigIntNum)) return ret;
		if (var()->bigIntNum.CompareTo(num.var()->bigIntNum) > 0)
		{
			ret.var()->sign = var()->sign;
			ret.var()->bigIntNum = var()->bigIntNum.Subtract(num.var()->bigIntNum);
		}
		else
		{
			ret.var()->sign = !num.var()->sign;
			ret.var()->bigIntNum = num.var()->bigIntNum.Subtract(var()->bigIntNum);
		}
	}
	return ret;
}
BigInteger BigInteger::operator *(BigInteger num)
{
	BigInteger ret;
	if (var()->bigIntNum.Equals(ret.var()->bigIntNum) || num.var()->bigIntNum.Equals(ret.var()->bigIntNum)) return ret;
	ret.var()->sign = var()->sign ? num.var()->sign : !num.var()->sign;
	ret.var()->bigIntNum = var()->bigIntNum.Multiply(num.var()->bigIntNum);
	return ret;
}
BigInteger BigInteger::operator /(BigInteger num)
{
	BigInteger ret;
	if(num.var()->bigIntNum.Equals(ret.var()->bigIntNum)) return ret;
	ret.var()->sign = var()->sign ? num.var()->sign : !num.var()->sign;
	ret.var()->bigIntNum = var()->bigIntNum.Divide(num.var()->bigIntNum);
	return ret;
}
BigInteger BigInteger::operator %(BigInteger num)
{
	BigInteger ret;
	if (num.var()->bigIntNum.Equals(ret.var()->bigIntNum)) return ret;
	ret.var()->sign = var()->sign;
	ret.var()->bigIntNum = var()->bigIntNum.Mod(num.var()->bigIntNum);
	return ret;
}

BigInteger BigInteger::modPow(BigInteger exponent, BigInteger modulus)
{
	BigInteger ret;
	if (exponent.var()->bigIntNum.Equals(ret.var()->bigIntNum) || modulus.var()->bigIntNum.Equals(ret.var()->bigIntNum)) return BigInteger();
	if (!exponent.isPositive() || !modulus.isPositive()) return BigInteger();

	if (var()->bigIntNum.Equals(ret.var()->bigIntNum)) return BigInteger();
	if (!var()->sign) return BigInteger();

	ret.var()->bigIntNum = var()->bigIntNum.ModPow(exponent.var()->bigIntNum, modulus.var()->bigIntNum);
	return ret;
}
