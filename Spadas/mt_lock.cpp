
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

#elif defined(SPADAS_ENV_LINUX)

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
		LockVars() : threadID(0)
		{
			initLock(&context);
		}
		~LockVars()
		{
			releaseLock(&context);
		}
	};
}

using namespace spadas;

const String spadas::Lock::TypeName = "spadas.Lock";

Lock::Lock() : Object<class LockVars>(new LockVars(), TRUE)
{
}

void Lock::enter()
{
	UInt threadID = Threads::getCurrentThreadID();
	SPADAS_ERROR_RETURN(vars->threadID == threadID);

	enterLock(&vars->context);
	vars->threadID = threadID;
}

void Lock::leave()
{
	SPADAS_ERROR_RETURN(vars->threadID != Threads::getCurrentThreadID());

	vars->threadID = 0;
	leaveLock(&vars->context);
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
