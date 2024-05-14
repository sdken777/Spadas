
#include "oscillator.h"

namespace oscillator_internal
{
    using namespace spadas;
    
	struct OscillatorEvent
	{
		Bool disposable;
		Bool toSet;
		ULong passed;
		ULong offset;
		ULong period;
		Flag trigger;
		OscillatorEvent *prev;
		OscillatorEvent *next;
		OscillatorEvent() : disposable(FALSE), toSet(FALSE), passed(ULINF), offset(ULINF), period(0), prev(NULL), next(NULL)
		{
		}
		OscillatorEvent(Bool disposable0, Bool toSet0, ULong period0, Flag trigger0, OscillatorEvent* prev0, OscillatorEvent* next0) : disposable(disposable0), toSet(toSet0), passed(ULINF), offset(ULINF), period(period0), trigger(trigger0), prev(prev0), next(next0)
		{
		}
	};
	
	struct DelayDelete
	{
		BaseObject obj;
		ULong timeoutTick;
		DelayDelete *next;
		DelayDelete() : timeoutTick(0), next(NULL)
		{
		}
	};

	class OscillatorVars : public Vars
	{
	public:
		Lock lock;
		OscillatorEvent *evHead, *evTail;
		DelayDelete *ddHead[(UInt)DelayDeleteTimeout::Count];
		DelayDelete *ddTail[(UInt)DelayDeleteTimeout::Count];
		Timer timer;
		OscillatorVars()
		{
			evHead = new OscillatorEvent();
			evTail = new OscillatorEvent();
			evHead->next = evTail;
			evTail->prev = evHead;
			for (UInt i = 0; i < (UInt)DelayDeleteTimeout::Count; i++) ddHead[i] = ddTail[i] = NULL;
			oscillatorID = startOscillator();
		}
		~OscillatorVars()
		{
			stopOscillator(oscillatorID);
			OscillatorEvent *ptr = evHead->next;
			while (ptr != evTail)
			{
				OscillatorEvent *nextPtr = ptr->next;
				delete ptr;
				ptr = nextPtr;
			}
			delete evHead;
			delete evTail;
			for (UInt i = 0; i < (UInt)DelayDeleteTimeout::Count; i++)
			{
				DelayDelete *node = ddHead[i];
				while (node)
				{
					DelayDelete *next = node->next;
					delete node;
					node = next;
				}
			}
		}
	private:
		UInt oscillatorID;
		UInt startOscillator();
		void stopOscillator(UInt id);
	};
	
	OscillatorManager::OscillatorManager()
	{
		oscillator = NULL;
	}
	OscillatorManager::~OscillatorManager()
	{
		if (oscillator) delete oscillator;
	}
	Oscillator *OscillatorManager::obj()
	{
		if (oscillator) return oscillator;
		lock.enter();
		if (!oscillator) oscillator = new Oscillator;
		lock.leave();
		return oscillator;
	}
	OscillatorManager om;
}

using namespace spadas;
using namespace oscillator_internal;

Oscillator::Oscillator() : Object<class OscillatorVars>(new OscillatorVars(), TRUE)
{
}

void Oscillator::add(Flag trigger, UInt period, Bool disposable, Bool toSet)
{
	vars->spinEnter();
	{
		OscillatorEvent* newEvent = new OscillatorEvent(disposable, toSet, (ULong)period, trigger, vars->evHead, vars->evHead->next);
		newEvent->prev->next = newEvent;
		newEvent->next->prev = newEvent;
	}
	vars->spinLeave();
}

void Oscillator::remove(Flag trigger)
{
	vars->spinEnter();
	{
		OscillatorEvent *ptr = vars->evHead->next;
		while (ptr != vars->evTail)
		{
			if (ptr->trigger == trigger)
			{
				OscillatorEvent *ptrNext = ptr->next;
				ptr = ptr->prev;
				delete ptr->next;
				ptr->next = ptrNext;
				ptrNext->prev = ptr;
				break;
			}
			ptr = ptr->next;
		}
	}
	vars->spinLeave();
}

void Oscillator::pulse()
{
	vars->spinEnter();
	{
		ULong count = (ULong)vars->timer.check();
		OscillatorEvent *ptr = vars->evHead->next;
		while (ptr != vars->evTail)
		{
			if (ptr->passed == ULINF)
			{
				ptr->offset = count % ptr->period;
				ptr->passed = count - ptr->offset;
			}
			else if (count - ptr->offset >= ptr->passed + ptr->period)
			{
				if (ptr->toSet) ptr->trigger.set();
				else ptr->trigger.reset();
				if (ptr->disposable)
				{
					OscillatorEvent *ptrNext = ptr->next;
					ptr = ptr->prev;
					delete ptr->next;
					ptr->next = ptrNext;
					ptrNext->prev = ptr;
				}
				else
				{
					ULong tmp = count - ptr->offset;
					ptr->passed = tmp - tmp % ptr->period;
				}
			}
			ptr = ptr->next;
		}

		ULong curTick = Timer::cpuTick();
		for (UInt i = 0; i < (UInt)DelayDeleteTimeout::Count; i++)
		{
			while (vars->ddHead[i])
			{
				if (vars->ddHead[i]->timeoutTick > curTick) break;
				DelayDelete *node = vars->ddHead[i];
				vars->ddHead[i] = vars->ddHead[i]->next;
				delete node;
			}
			if (!vars->ddHead[i])
			{
				vars->ddTail[i] = NULL;
			}
		}
	}
	vars->lock.leave();
}

void Oscillator::delayDelete(BaseObject obj, DelayDeleteTimeout timeout)
{
	if (obj.isNull()) return;

	ULong timeoutTicks = 0;
	switch (timeout)
	{
	case DelayDeleteTimeout::TenSeconds:
		timeoutTicks = Timer::cpuTicksPerSecond() * 10;
		break;
	default:
		return;
	}

	DelayDelete *newNode = new DelayDelete();
	newNode->obj = obj;
	newNode->timeoutTick = Timer::cpuTick() + timeoutTicks;

	vars->lock.enter();
	{
		if (vars->ddHead[(UInt)timeout])
		{
			vars->ddTail[(UInt)timeout]->next = newNode;
			vars->ddTail[(UInt)timeout] = newNode;
		}
		else
		{
			vars->ddHead[(UInt)timeout] = vars->ddTail[(UInt)timeout] = newNode;
		}
	}
	vars->spinLeave();
}

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>

void oscillator_internal::sleepTime(spadas::UInt time)
{
	Sleep((DWORD)time);
}

void __stdcall oscillatorCallback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	om.obj()->pulse();
}

UInt OscillatorVars::startOscillator()
{
	TIMECAPS tickCaps;
	ZeroMemory(&tickCaps, sizeof(TIMECAPS));
	timeGetDevCaps(&tickCaps, sizeof(TIMECAPS));
		
	UInt timerID = timeSetEvent(1, tickCaps.wPeriodMin, oscillatorCallback, 0, TIME_PERIODIC);
		
	if (timerID == 0)
	{
		SPADAS_ERROR_MSG("timerID == 0");
		system::exit();
		return 0;
	}
	return timerID;
}

void OscillatorVars::stopOscillator(UInt id)
{
	timeKillEvent(id);
}

#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)

#include <pthread.h>
#include <sys/time.h>
#undef NULL
#define NULL 0

void oscillator_internal::sleepTime(spadas::UInt time)
{
	timeval interval;
	interval.tv_sec = 0;
	interval.tv_usec = time * 1000;
	select(0, NULL, NULL, NULL, &interval);
}

volatile Bool oscillatorShouldEnd = FALSE;
Pointer oscillatorThreadFunc(Pointer param)
{
	while (!oscillatorShouldEnd)
	{
		om.obj()->pulse();
		sleepTime(1);
	}
	return 0;
}

UInt OscillatorVars::startOscillator()
{
	sched_param sched;
	sched.sched_priority = 99;
	
	pthread_attr_t threadAttri;
	pthread_attr_init(&threadAttri);
	pthread_attr_setschedparam(&threadAttri, &sched);
	pthread_attr_setdetachstate(&threadAttri, PTHREAD_CREATE_DETACHED);
	
	pthread_t thread;
	pthread_create(&thread, &threadAttri, oscillatorThreadFunc, NULL);
	
	pthread_attr_destroy(&threadAttri);
	
	return 0;
}
void OscillatorVars::stopOscillator(UInt id)
{
	oscillatorShouldEnd = TRUE;
	sleepTime(10);
}

#endif
