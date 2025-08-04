
#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>
#include "oscillator.h"

namespace spadas
{
	class FlagVars : public Vars
	{
    public:
		HANDLE hEventSet;
		HANDLE hEventReset;
        FlagVars() : hEventSet(0), hEventReset(0)
        {
			hEventSet = CreateEvent(0, 1, 0, 0);
			hEventReset = CreateEvent(0, 1, 1, 0);
        }
        ~FlagVars()
        {
			CloseHandle(hEventSet);
			CloseHandle(hEventReset);
        }
		Bool isSpinLockManaged() override
		{
			return TRUE;
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
using namespace oscillator_internal;

Bool Flag::check()
{
	return WaitForSingleObject(var()->hEventSet, 0) == WAIT_OBJECT_0;
}

Bool Flag::waitSet(UInt waitTime)
{
	return WaitForSingleObject(var()->hEventSet, waitTime) != WAIT_TIMEOUT;
}

Bool Flag::waitReset(UInt waitTime)
{
	return WaitForSingleObject(var()->hEventReset, waitTime) != WAIT_TIMEOUT;
}

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#undef NULL

#include "oscillator.h"

namespace spadas
{
	class FlagVars : public Vars
	{
    public:
		SPADAS_VARS(Flag, Vars)

		volatile Bool flag;
		volatile Bool waitingSet, waitingReset;
		pthread_mutex_t mutex;
		pthread_cond_t setSignal, resetSignal;
		
        FlagVars() : flag(FALSE), waitingSet(FALSE), waitingReset(FALSE)
        {
            pthread_mutex_init(&mutex, 0);
            pthread_cond_init(&setSignal, 0);
            pthread_cond_init(&resetSignal, 0);
        }
        ~FlagVars()
        {
            pthread_cond_destroy(&setSignal);
            pthread_cond_destroy(&resetSignal);
            pthread_mutex_destroy(&mutex);
        }
		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
        void set()
        {
            pthread_mutex_lock(&mutex);
			if (!flag)
			{
				flag = TRUE;
				if (waitingSet)
				{
					waitingSet = FALSE;
					pthread_cond_broadcast(&setSignal);
				}
			}
            pthread_mutex_unlock(&mutex);
        }
        void reset()
        {
            pthread_mutex_lock(&mutex);
			if (flag)
			{
				flag = FALSE;
				if (waitingReset)
				{
					waitingReset = FALSE;
					pthread_cond_broadcast(&resetSignal);
				}
			}
            pthread_mutex_unlock(&mutex);
        }
	};
}

using namespace spadas;
using namespace oscillator_internal;

Bool Flag::waitSet(UInt waitTime)
{
	int ret;
	pthread_mutex_lock(&var()->mutex);
	{
		if (var()->flag)
		{
			pthread_mutex_unlock(&var()->mutex);
			return TRUE;
		}
		
		timeval target;
		gettimeofday(&target, 0);
		ULong targetMicro = (ULong)target.tv_usec + (ULong)waitTime * 1000ull;

		timespec timeInfo;
		timeInfo.tv_sec = target.tv_sec + (time_t)(targetMicro / 1000000ull);
		timeInfo.tv_nsec = (time_t)(targetMicro % 1000000ull) * 1000;

		var()->waitingSet = TRUE;
		ret = pthread_cond_timedwait(&var()->setSignal, &var()->mutex, &timeInfo);
	}
	pthread_mutex_unlock(&var()->mutex);

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
	pthread_mutex_lock(&var()->mutex);
	{
		if (!var()->flag)
		{
			pthread_mutex_unlock(&var()->mutex);
			return TRUE;
		}
		
		timeval target;
		gettimeofday(&target, 0);
		ULong targetMicro = (ULong)target.tv_usec + (ULong)waitTime * 1000ull;

		timespec timeInfo;
		timeInfo.tv_sec = target.tv_sec + (time_t)(targetMicro / 1000000ull);
		timeInfo.tv_nsec = (time_t)(targetMicro % 1000000ull) * 1000;

		var()->waitingReset = TRUE;
		ret = pthread_cond_timedwait(&var()->resetSignal, &var()->mutex, &timeInfo);
	}
	pthread_mutex_unlock(&var()->mutex);

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
	return var()->flag;
}

#endif

Flag::Flag() : BaseObject(new FlagVars())
{
}

Flag::Flag(UInt time) : BaseObject(new FlagVars())
{
	delaySet(time);
}

void Flag::set()
{
    var()->set();
}

void Flag::reset()
{
    var()->reset();
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
