
#include "spadas.h"

namespace spadas
{
	class MagicNumberVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(MagicNumber, Vars)

		Char magic[4];
		MagicNumberVars()
		{
			*(UInt*)magic = 0;
		}
		String toString() override
		{
			return Binary((Byte*)magic, 4);
		}
	};
}

using namespace spadas;

const String spadas::MagicNumber::TypeName = "spadas.MagicNumber";

MagicNumber::MagicNumber()
{

}

MagicNumber::MagicNumber(Char c0, Char c1, Char c2, Char c3)
{
	Char cs[4] = { c0, c1, c2, c3 };
	for (UInt i = 0; i < 4; i++)
	{
		Char c = cs[i];
		Bool ok = FALSE;
		if (c >= '0' && c <= '9') ok = TRUE;
		else if (c >= 'A' && c <= 'Z') ok = TRUE;
		else if (c >= 'a' && c <= 'z')
		{
			cs[i] -= 32;
			ok = TRUE;
		}
		if (!ok) return;
	}
	setVars(new MagicNumberVars(), TRUE);
	for (UInt i = 0; i < 4; i++) vars->magic[i] = cs[i];
}

Bool MagicNumber::operator ==(MagicNumber magic)
{
	return getNumber() == magic.getNumber();
}
Bool MagicNumber::operator !=(MagicNumber magic)
{
	return getNumber() != magic.getNumber();
}

Array<Char> MagicNumber::getChars()
{
	if (!vars) return Array<Char>(4, 0);
	return Array<Char>(vars->magic, 4);
}

UInt MagicNumber::getNumber()
{
	if (!vars) return 0;
	return *(UInt*)vars->magic;
}

Word MagicNumber::getHash()
{
	if (!vars) return 0;
	return (Byte)vars->magic[0] ^ (Byte)vars->magic[1] ^ (Byte)vars->magic[2] ^ (Byte)vars->magic[3];
}

String MagicNumber::toString()
{
	if (!vars) return String();
	else return Binary((Byte*)vars->magic, 4);
}
