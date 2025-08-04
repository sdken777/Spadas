
#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>
#include "spadas.h"

typedef CRITICAL_SECTION LockContext;
void initLock(CRITICAL_SECTION *context)
{
	InitializeCriticalSection(context);
}
void releaseLock(CRITICAL_SECTION *context)
{
	DeleteCriticalSection(context);
}
void enterLock(CRITICAL_SECTION *context)
{
	EnterCriticalSection(context);
}
void leaveLock(CRITICAL_SECTION *context)
{
	LeaveCriticalSection(context);
}

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <pthread.h>
#undef NULL

#include "spadas.h"

typedef pthread_mutex_t LockContext;
void initLock(pthread_mutex_t *context)
{
	pthread_mutex_init(context, 0);
}
void releaseLock(pthread_mutex_t *context)
{
	pthread_mutex_destroy(context);
}
void enterLock(pthread_mutex_t *context)
{
	pthread_mutex_lock(context);
}
void leaveLock(pthread_mutex_t *context)
{
	pthread_mutex_unlock(context);
}

#endif

namespace spadas
{
	class LockVars : public Vars
	{
	public:
		SPADAS_VARS(Lock, Vars)
		volatile UInt threadID;
		LockContext context;
		Bool isSpin;
		LockVars(Bool isSpin) : threadID(0), isSpin(isSpin)
		{
			if (!isSpin) initLock(&context);
		}
		~LockVars()
		{
			if (!isSpin) releaseLock(&context);
		}
		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
	};
}

using namespace spadas;

Lock::Lock() : BaseObject(new LockVars(FALSE))
{
}

Lock::Lock(Bool spin) : BaseObject(new LockVars(spin))
{
}

Bool Lock::isSpin()
{
	return var()->isSpin;
}

void Lock::enter()
{
	UInt threadID = Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(threadID == 0);
	SPADAS_ERROR_RETURN(var()->threadID == threadID);

	if (var()->isSpin) var()->spinEnter();
	else enterLock(&var()->context);

	var()->threadID = threadID;
}

void Lock::leave()
{
	UInt threadID = Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(threadID == 0);
	SPADAS_ERROR_RETURN(var()->threadID != threadID);
	var()->threadID = 0;

	if (var()->isSpin) var()->spinLeave();
	else leaveLock(&var()->context);
}

LockProxy::LockProxy(Lock& lock0) : lock(lock0), released(FALSE)
{
	lock.enter();
}

LockProxy::~LockProxy()
{
	if (!released) lock.leave();
}

void LockProxy::releaseLock()
{
	if (!released)
	{
		lock.leave();
		released = TRUE;
	}
}
