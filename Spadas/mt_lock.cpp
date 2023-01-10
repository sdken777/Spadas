
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

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <pthread.h>

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
		SPADAS_VARS_DEF(Lock, Vars)

		volatile UInt threadID;
		LockContext context;
		Atom spinLock;
		Bool isSpin;
		LockVars(Bool isSpin) : threadID(0), isSpin(isSpin)
		{
			if (!isSpin) initLock(&context);
		}
		~LockVars()
		{
			if (!isSpin) releaseLock(&context);
		}
	};
}

using namespace spadas;

const String spadas::Lock::TypeName = "spadas.Lock";

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
	if (vars->isSpin)
	{
		while (!vars->spinLock.cas(0, 1)) {}
	}
	else
	{
		UInt threadID = Threads::getCurrentThreadID();
		SPADAS_ERROR_RETURN(vars->threadID == threadID);
		enterLock(&vars->context);
		vars->threadID = threadID;
	}
}

void Lock::leave()
{
	if (vars->isSpin)
	{
		while (TRUE)
		{
			Int oldVal = vars->spinLock.get();
			if (oldVal != 1 || vars->spinLock.cas(1, 0)) break;
		}
	}
	else
	{
		SPADAS_ERROR_RETURN(vars->threadID != Threads::getCurrentThreadID());
		vars->threadID = 0;
		leaveLock(&vars->context);
	}
}

LockProxy::LockProxy(Lock lock0) : lock(lock0), released(FALSE)
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
