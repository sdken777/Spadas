#define SPADAS_ENABLE_MACROS
#include "spadas.h"

namespace utility_internal
{
	using namespace spadas;

	Dictionary<BaseObject> sharedObjects(256);
	Lock sharedObjectsLock;
}

using namespace spadas;
using namespace utility_internal;

BaseObject spadas::utility::getSharedObject(String key, Func<BaseObject()> defaultCreator)
{
	SPADAS_ERROR_RETURNVAL(key.isEmpty(), BaseObject());
	SPADAS_ERROR_RETURNVAL(!defaultCreator, BaseObject());

	LockProxy p(sharedObjectsLock);
	BaseObject target;
	if (!sharedObjects.tryGet(key, target))
	{
		target = defaultCreator();
		SPADAS_ERROR_RETURNVAL(target.isNull(), BaseObject());
		sharedObjects[key] = target;
	}
	return target;
}