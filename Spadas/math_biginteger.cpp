
#include "spadas.h"
#include "3party/BigIntegerWrapper.h"

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

BigInteger::BigInteger() : Object<BigIntegerVars>(new BigIntegerVars, TRUE)
{
	vars->bigIntNum = std::string("0");
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
		vars->bigIntNum = std::string("0");
		vars->sign = TRUE;
		return;
	}

	std::vector<uint8_t> vector(integerData.size() - validIndex);
	for (UInt i = validIndex; i < integerData.size(); i++) vector.push_back(integerData[i]);
	vars->bigIntNum = vector;

	vars->sign = vars->bigIntNum.Equals(std::string("0")) ? TRUE : sign;
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
	ret.vars->sign = !isNegative;
	ret.vars->bigIntNum = std::string((Char*)&stringData[isNegative ? 1 : 0]);
	return ret;
}

Bool BigInteger::isPositive()
{
	return vars->sign;
}

Binary BigInteger::getData()
{
	std::vector<uint8_t> vector = vars->bigIntNum.ToByteArrayUnsigned();

	Binary output(vector.size());
	for (UInt i = 0; i < output.size(); i++) output[i] = vector[i];
	return output;
}

Word BigInteger::getHash()
{
	return vars->toString().getHash();
}

Bool BigInteger::operator ==(BigInteger num)
{
	return vars->sign == num.vars->sign && vars->bigIntNum.Equals(num.vars->bigIntNum);
}
Bool BigInteger::operator !=(BigInteger num)
{
	return vars->sign != num.vars->sign || !vars->bigIntNum.Equals(num.vars->bigIntNum);
}
Bool BigInteger::operator <(BigInteger num)
{
	if (vars->sign == TRUE && num.vars->sign == FALSE) return FALSE;
	else if (vars->sign == FALSE && num.vars->sign == TRUE) return TRUE;
	else if (vars->sign == TRUE) return vars->bigIntNum.CompareTo(num.vars->bigIntNum) < 0;
	else return vars->bigIntNum.CompareTo(num.vars->bigIntNum) > 0;
}
Bool BigInteger::operator >(BigInteger num)
{
	if (vars->sign == TRUE && num.vars->sign == FALSE) return TRUE;
	else if (vars->sign == FALSE && num.vars->sign == TRUE) return FALSE;
	else if (vars->sign == TRUE) return vars->bigIntNum.CompareTo(num.vars->bigIntNum) > 0;
	else return vars->bigIntNum.CompareTo(num.vars->bigIntNum) < 0;
}

BigInteger BigInteger::operator +(BigInteger num)
{
	BigInteger ret;
	if (vars->sign == num.vars->sign)
	{
		ret.vars->sign = vars->sign;
		ret.vars->bigIntNum = vars->bigIntNum.Add(num.vars->bigIntNum);
	}
	else
	{
		if (vars->bigIntNum.Equals(num.vars->bigIntNum)) return ret;
		if (vars->bigIntNum.CompareTo(num.vars->bigIntNum) > 0)
		{
			ret.vars->sign = vars->sign;
			ret.vars->bigIntNum = vars->bigIntNum.Subtract(num.vars->bigIntNum);
		}
		else
		{
			ret.vars->sign = num.vars->sign;
			ret.vars->bigIntNum = num.vars->bigIntNum.Subtract(vars->bigIntNum);
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
		ret.vars->bigIntNum = vars->bigIntNum.Add(num.vars->bigIntNum);
	}
	else
	{
		if (vars->bigIntNum.Equals(num.vars->bigIntNum)) return ret;
		if (vars->bigIntNum.CompareTo(num.vars->bigIntNum) > 0)
		{
			ret.vars->sign = vars->sign;
			ret.vars->bigIntNum = vars->bigIntNum.Subtract(num.vars->bigIntNum);
		}
		else
		{
			ret.vars->sign = !num.vars->sign;
			ret.vars->bigIntNum = num.vars->bigIntNum.Subtract(vars->bigIntNum);
		}
	}
	return ret;
}
BigInteger BigInteger::operator *(BigInteger num)
{
	BigInteger ret;
	if (vars->bigIntNum.Equals(ret.vars->bigIntNum) || num.vars->bigIntNum.Equals(ret.vars->bigIntNum)) return ret;
	ret.vars->sign = vars->sign ? num.vars->sign : !num.vars->sign;
	ret.vars->bigIntNum = vars->bigIntNum.Multiply(num.vars->bigIntNum);
	return ret;
}
BigInteger BigInteger::operator /(BigInteger num)
{
	BigInteger ret;
	if(num.vars->bigIntNum.Equals(ret.vars->bigIntNum)) return ret;
	ret.vars->sign = vars->sign ? num.vars->sign : !num.vars->sign;
	ret.vars->bigIntNum = vars->bigIntNum.Divide(num.vars->bigIntNum);
	return ret;
}
BigInteger BigInteger::operator %(BigInteger num)
{
	BigInteger ret;
	if (num.vars->bigIntNum.Equals(ret.vars->bigIntNum)) return ret;
	ret.vars->sign = vars->sign;
	ret.vars->bigIntNum = vars->bigIntNum.Mod(num.vars->bigIntNum);
	return ret;
}

BigInteger BigInteger::modPow(BigInteger exponent, BigInteger modulus)
{
	BigInteger ret;
	if (exponent.vars->bigIntNum.Equals(ret.vars->bigIntNum) || modulus.vars->bigIntNum.Equals(ret.vars->bigIntNum)) return BigInteger();
	if (!exponent.isPositive() || !modulus.isPositive()) return BigInteger();

	if (vars->bigIntNum.Equals(ret.vars->bigIntNum)) return BigInteger();
	if (!vars->sign) return BigInteger();

	ret.vars->bigIntNum = vars->bigIntNum.ModPow(exponent.vars->bigIntNum, modulus.vars->bigIntNum);
	return ret;
}
