
#include "spadas.h"

using namespace spadas;

Atom globalObjectCounter;
const String BaseObject::TypeName = "spadas.BaseObject";

Vars::Vars() :nRefs(1)
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

UInt Vars::getObjectCount()
{
	return (UInt)math::max(0, globalObjectCounter.get());
}

String Vars::getTypeName()
{
	return BaseObject::TypeName;
}

ListNode<String> Vars::getBaseChain()
{
	return ListNode<String>();
}

ListNode<String> Vars::genBaseChain(String baseType, ListNode<String> baseBaseChain)
{
	if (baseType.isEmpty()) return ListNode<String>();
	if (!baseBaseChain->isEmpty()) return baseBaseChain.joinPrevious(baseType);
	else return ListNode<String>(baseType);
}

String Vars::toString()
{
	return getTypeName() + " object";
}