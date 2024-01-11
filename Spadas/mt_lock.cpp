

#include "spadas.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>

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

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)

#include <pthread.h>
#undef NULL
#define NULL 0

typedef pthread_mutex_t LockContext;
void initLock(pthread_mutex_t *context)
{
	pthread_mutex_init(context, NULL);
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

Lock::Lock() : Object<class LockVars>(new LockVars(FALSE), TRUE)
{
}

Lock::Lock(Bool spin) : Object<class LockVars>(new LockVars(spin), TRUE)
{
}

Bool Lock::isSpin()
{
	return vars->isSpin;
}

void Lock::enter()
{
	UInt threadID = Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(threadID == 0);
	SPADAS_ERROR_RETURN(vars->threadID == threadID);

	if (vars->isSpin) vars->spinEnter();
	else enterLock(&vars->context);

	vars->threadID = threadID;
}

void Lock::leave()
{
	UInt threadID = Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(threadID == 0);
	SPADAS_ERROR_RETURN(vars->threadID != threadID);
	vars->threadID = 0;

	if (vars->isSpin) vars->spinLeave();
	else leaveLock(&vars->context);
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
