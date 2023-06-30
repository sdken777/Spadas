
#include "spadas.h"

using namespace spadas;

Atom globalObjectCounter;
String objectNamePostfix = " object";

const String BaseObject::TypeName = "spadas.BaseObject";

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
	while (!varSpinLock.cas(0, 1)) {}
}

void Vars::spinLeave()
{
	varSpinLock.set(0);
}

UInt Vars::getObjectCount()
{
	return (UInt)math::max(0, globalObjectCounter.get());
}

String Vars::getTypeName()
{
	return BaseObject::TypeName;
}

Bool Vars::isType(ULong typeID)
{
	return typeID == BaseObject::TypeName.getID();
}

Bool Vars::isType(String typeName)
{
	return typeName == BaseObject::TypeName;
}

String Vars::toString()
{
	return getTypeName() + objectNamePostfix;
}

Bool Vars::isSpinLockManaged()
{
	return FALSE;
}