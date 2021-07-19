
#include "spadas.h"

#include "oscillator.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <Windows.h>

namespace spadas
{
	class FlagVars : public Vars
	{
    public:
		HANDLE hEventSet;
		HANDLE hEventReset;
        FlagVars() : hEventSet(FALSE), hEventReset(FALSE)
        {
			hEventSet = CreateEvent(0, 1, 0, 0);
			hEventReset = CreateEvent(0, 1, 1, 0);
        }
        ~FlagVars()
        {
			CloseHandle(hEventSet);
			CloseHandle(hEventReset);
        }
		void set()
		{
			SetEvent(hEventSet);
			ResetEvent(hEventReset);
		}
		void reset()
		{
			ResetEvent(hEventSet);
			SetEvent(hEventReset);
		}
	};
}

using namespace spadas;
using namespace spadas_internal;

Bool Flag::check()
{
	return WaitForSingleObject(vars->hEventSet, 0) == WAIT_OBJECT_0;
}

Bool Flag::waitSet(UInt waitTime)
{
	return WaitForSingleObject(vars->hEventSet, waitTime) != WAIT_TIMEOUT;
}

Bool Flag::waitReset(UInt waitTime)
{
	return WaitForSingleObject(vars->hEventReset, waitTime) != WAIT_TIMEOUT;
}

#elif defined(SPADAS_ENV_LINUX)

#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

namespace spadas
{
	class FlagVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(Flag, Vars)

		volatile Bool flag;
		volatile Bool waitingSet, waitingReset;
		pthread_mutex_t lock;
		pthread_cond_t setSignal, resetSignal;
		
        FlagVars() : flag(FALSE), waitingSet(FALSE), waitingReset(FALSE)
        {
            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&setSignal, NULL);
            pthread_cond_init(&resetSignal, NULL);
        }
        ~FlagVars()
        {
            pthread_cond_destroy(&setSignal);
            pthread_cond_destroy(&resetSignal);
            pthread_mutex_destroy(&lock);
        }
        void set()
        {
            pthread_mutex_lock(&lock);
			if (!flag)
			{
				flag = TRUE;
				if (waitingSet)
				{
					waitingSet = FALSE;
					pthread_cond_broadcast(&setSignal);
				}
			}
            pthread_mutex_unlock(&lock);
        }
        void reset()
        {
            pthread_mutex_lock(&lock);
			if (flag)
			{
				flag = FALSE;
				if (waitingReset)
				{
					waitingReset = FALSE;
					pthread_cond_broadcast(&resetSignal);
				}
			}
            pthread_mutex_unlock(&lock);
        }
	};
}

using namespace spadas;
using namespace spadas_internal;

Bool Flag::waitSet(UInt waitTime)
{
	int ret;
	pthread_mutex_lock(&vars->lock);
	{
		if (vars->flag)
		{
			pthread_mutex_unlock(&vars->lock);
			return TRUE;
		}
		
		timeval target;
		gettimeofday(&target, NULL);
		ULong targetMicro = (ULong)target.tv_usec + (ULong)waitTime * 1000ull;

		timespec timeInfo;
		timeInfo.tv_sec = target.tv_sec + (time_t)(targetMicro / 1000000ull);
		timeInfo.tv_nsec = (time_t)(targetMicro % 1000000ull) * 1000;

		vars->waitingSet = TRUE;
		ret = pthread_cond_timedwait(&vars->setSignal, &vars->lock, &timeInfo);
	}
	pthread_mutex_unlock(&vars->lock);

	if (ret == 0) return TRUE;
	else if (ret == ETIMEDOUT) return FALSE;
	else
	{
		SPADAS_ERROR_MSG("Invalid ret");
		return FALSE;
	}
}

Bool Flag::waitReset(UInt waitTime)
{
	int ret = 0;
	pthread_mutex_lock(&vars->lock);
	{
		if (!vars->flag)
		{
			pthread_mutex_unlock(&vars->lock);
			return TRUE;
		}
		
		timeval target;
		gettimeofday(&target, NULL);
		ULong targetMicro = (ULong)target.tv_usec + (ULong)waitTime * 1000ull;

		timespec timeInfo;
		timeInfo.tv_sec = target.tv_sec + (time_t)(targetMicro / 1000000ull);
		timeInfo.tv_nsec = (time_t)(targetMicro % 1000000ull) * 1000;

		vars->waitingReset = TRUE;
		ret = pthread_cond_timedwait(&vars->resetSignal, &vars->lock, &timeInfo);
	}
	pthread_mutex_unlock(&vars->lock);

	if (ret == 0) return TRUE;
	else if (ret == ETIMEDOUT) return FALSE;
	else
	{
		SPADAS_ERROR_MSG("Invalid ret");
		return FALSE;
	}
}

Bool Flag::check()
{
	return vars->flag;
}

#endif

const String spadas::Flag::TypeName = "spadas.Flag";

Flag::Flag() : Object<class FlagVars>(new FlagVars(), TRUE)
{
}

Flag::Flag(UInt time) : Object<class FlagVars>(new FlagVars(), TRUE)
{
	delaySet(time);
}

void Flag::set()
{
    vars->set();
}

void Flag::reset()
{
    vars->reset();
}

void Flag::delaySet(UInt time)
{
	if (time == 0) set();
	else om.obj()->add(*this, time, TRUE, TRUE);
}

void Flag::delayReset(UInt time)
{
	if (time == 0) reset();
	else om.obj()->add(*this, time, TRUE, FALSE);
}
