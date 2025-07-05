
#include "spadas.h"

using namespace spadas;

Atom globalObjectCounter;
String objectNamePostfix = " object";

String BaseObject::typeName() { static String name = "spadas.BaseObject"; return name; }

Vars::Vars() : nRefs(1)
{
	globalObjectCounter.increase();
}
Vars::~Vars()
{
	globalObjectCounter.decrease();
}
void Vars::retain()
{
	nRefs.increase();
}
void Vars::release()
{
	if (nRefs.decrease() == 0) delete this;
}
UInt Vars::getRefCount()
{
	return (UInt)nRefs.get();
}

void Vars::spinEnter()
{
	varSpinLock.casSpin(0, 1);
}

void Vars::spinLeave()
{
#if defined(SPADAS_ARCH_X86)
	varSpinLock.set(0);
#else // SPADAS_ARCH_ARM
	varSpinLock.cas(1, 0);
#endif
}

UInt Vars::getObjectCount()
{
	return (UInt)math::max(0, globalObjectCounter.get());
}

String Vars::getTypeName()
{
	return BaseObject::typeName();
}

Bool Vars::isType(ULong id)
{
	return id == BaseObject::typeName().getID();
}

Bool Vars::isType(String name)
{
	return name == BaseObject::typeName();
}

String Vars::toString()
{
	return getTypeName() + objectNamePostfix;
}

Bool Vars::isSpinLockManaged()
{
	return FALSE;
}