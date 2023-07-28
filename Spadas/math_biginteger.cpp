
#include "spadas.h"
#include "3party/BigInt.hpp"
#include "3party/constructors/constructors.hpp"
#include "3party/functions/conversion.hpp"
#include "3party/functions/math.hpp"
#include "3party/functions/random.hpp"
#include "3party/functions/utility.hpp"
#include "3party/operators/arithmetic_assignment.hpp"
#include "3party/operators/assignment.hpp"
#include "3party/operators/binary_arithmetic.hpp"
#include "3party/operators/increment_decrement.hpp"
#include "3party/operators/io_stream.hpp"
#include "3party/operators/relational.hpp"
#include "3party/operators/unary_arithmetic.hpp"

namespace spadas
{
	class BigIntegerVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(BigInteger, Vars)

		Bool sign; // TRUE: positive or zero, FALSE: negative
		BigInt bigIntNum;

		String toString() override
		{
			String numString(bigIntNum.to_string().c_str());
			if (sign) return numString;
			else return (String)"-" + numString;
		}
	};
}

using namespace spadas;

const String spadas::BigInteger::TypeName = "spadas.BigInteger";

BigInteger::BigInteger() : Object<BigIntegerVars>(new BigIntegerVars, TRUE)
{
	vars->bigIntNum = 0;
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
		vars->bigIntNum = 0;
		vars->sign = TRUE;
		return;
	}

	vars->bigIntNum = 0;
	for (UInt i = validIndex; i < integerData.size(); i++)
	{
		vars->bigIntNum *= 256;
		vars->bigIntNum += (Long)integerData[i];
	}

	vars->sign = vars->bigIntNum == 0 ? TRUE : sign;
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
	ret.vars->bigIntNum = BigInt((Char*)&stringData[isNegative ? 1 : 0]);
	return ret;
}

Bool BigInteger::isPositive()
{
	return vars->sign;
}

Binary BigInteger::getData()
{
	ArrayX<Byte> bytes;

	BigInt num = vars->bigIntNum;
	while (num != 0)
	{
		bytes.append((Byte)(num % 256).to_int());
		num /= 256;
	}

	if (bytes.isEmpty()) return Binary(1, 0);

	Binary ret(bytes.toArray().data(), bytes.size());
	return ret.reverse();
}

String BigInteger::toString()
{
	return vars->toString();
}

Word BigInteger::getHash()
{
	return vars->toString().getHash();
}

Bool BigInteger::operator ==(BigInteger num)
{
	return vars->sign == num.vars->sign && vars->bigIntNum == num.vars->bigIntNum;
}
Bool BigInteger::operator !=(BigInteger num)
{
	return vars->sign != num.vars->sign || vars->bigIntNum != num.vars->bigIntNum;
}
Bool BigInteger::operator <(BigInteger num)
{
	if (vars->sign == TRUE && num.vars->sign == FALSE) return FALSE;
	else if (vars->sign == FALSE && num.vars->sign == TRUE) return TRUE;
	else if (vars->sign == TRUE) return vars->bigIntNum < num.vars->bigIntNum;
	else return vars->bigIntNum > num.vars->bigIntNum;
}
Bool BigInteger::operator >(BigInteger num)
{
	if (vars->sign == TRUE && num.vars->sign == FALSE) return TRUE;
	else if (vars->sign == FALSE && num.vars->sign == TRUE) return FALSE;
	else if (vars->sign == TRUE) return vars->bigIntNum > num.vars->bigIntNum;
	else return vars->bigIntNum < num.vars->bigIntNum;
}

BigInteger BigInteger::operator +(BigInteger num)
{
	BigInteger ret;
	if (vars->sign == num.vars->sign)
	{
		ret.vars->sign = vars->sign;
		ret.vars->bigIntNum = vars->bigIntNum + num.vars->bigIntNum;
	}
	else
	{
		if (vars->bigIntNum == num.vars->bigIntNum) return ret;
		if (vars->bigIntNum > num.vars->bigIntNum)
		{
			ret.vars->sign = vars->sign;
			ret.vars->bigIntNum = vars->bigIntNum - num.vars->bigIntNum;
		}
		else
		{
			ret.vars->sign = num.vars->sign;
			ret.vars->bigIntNum = num.vars->bigIntNum - vars->bigIntNum;
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
		ret.vars->bigIntNum = vars->bigIntNum + num.vars->bigIntNum;
	}
	else
	{
		if (vars->bigIntNum == num.vars->bigIntNum) return ret;
		if (vars->bigIntNum > num.vars->bigIntNum)
		{
			ret.vars->sign = vars->sign;
			ret.vars->bigIntNum = vars->bigIntNum - num.vars->bigIntNum;
		}
		else
		{
			ret.vars->sign = !num.vars->sign;
			ret.vars->bigIntNum = num.vars->bigIntNum - vars->bigIntNum;
		}
	}
	return ret;
}
BigInteger BigInteger::operator *(BigInteger num)
{
	BigInteger ret;
	if (vars->bigIntNum == ret.vars->bigIntNum || num.vars->bigIntNum == ret.vars->bigIntNum) return ret;
	ret.vars->sign = vars->sign ? num.vars->sign : !num.vars->sign;
	ret.vars->bigIntNum = vars->bigIntNum * num.vars->bigIntNum;
	return ret;
}
BigInteger BigInteger::operator /(BigInteger num)
{
	BigInteger ret;
	SPADAS_ERROR_RETURNVAL(num.vars->bigIntNum == ret.vars->bigIntNum, ret);
	ret.vars->sign = vars->sign ? num.vars->sign : !num.vars->sign;
	ret.vars->bigIntNum = vars->bigIntNum / num.vars->bigIntNum;
	return ret;
}
BigInteger BigInteger::operator %(BigInteger num)
{
	BigInteger ret;
	SPADAS_ERROR_RETURNVAL(num.vars->bigIntNum == ret.vars->bigIntNum, ret);
	ret.vars->sign = vars->sign;
	ret.vars->bigIntNum = vars->bigIntNum % num.vars->bigIntNum;
	return ret;
}

BigInteger BigInteger::modPow(BigInteger exponent, BigInteger modulus)
{
	SPADAS_ERROR_RETURNVAL(exponent.vars->bigIntNum == 0 || modulus.vars->bigIntNum == 0, BigInteger());
	SPADAS_ERROR_RETURNVAL(!exponent.isPositive() || !modulus.isPositive(), BigInteger());

	if (vars->bigIntNum == 0) return BigInteger();
	SPADAS_ERROR_RETURNVAL(!vars->sign, BigInteger());

	BigInt res = 1;
	BigInt a = exponent.vars->bigIntNum;
	BigInt m = modulus.vars->bigIntNum;
	for (BigInt p = vars->bigIntNum; a != 0; a /= 2, p = (p * p) % m)
	{
		if (a % 2 != 0) res = (res * p) % m;
	}
	
	BigInteger ret;
	ret.vars->bigIntNum = res;
	return ret;
}
