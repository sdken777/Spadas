
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
	Int curThreadID = (Int)Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(curThreadID == 0);
	SPADAS_ERROR_RETURN(curThreadID == varSpinLock.get());
	while (!varSpinLock.cas(0, curThreadID)) {}
}

void Vars::spinLeave()
{
	Int curThreadID = (Int)Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(curThreadID == 0);
	SPADAS_ERROR_RETURN(curThreadID != varSpinLock.get());
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