
#include "spadas.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <windows.h>

namespace timer_interval
{
	using namespace spadas;

	inline ULong getCurrentCPUTick()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return (ULong)time.QuadPart;
	}

	inline ULong getCPUTicksPerSecond()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return (ULong)frequency.QuadPart;
	}
}

#endif

#if defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_NILRT)

#include <time.h>

namespace timer_interval
{
	using namespace spadas;

	inline ULong getCurrentCPUTick()
	{
		timespec time;
		clock_gettime(CLOCK_MONOTONIC_RAW, &time);
		return (ULong)time.tv_nsec + (ULong)time.tv_sec * 1000000000ull;
	}

	inline ULong getCPUTicksPerSecond()
	{
		return 1000000000ull;
	}
}

#endif

#if defined(SPADAS_ENV_MACOS)

#include <mach/mach_time.h>

namespace timer_interval
{
	using namespace spadas;

	inline ULong getCurrentCPUTick()
	{
		return mach_absolute_time();
	}

	inline ULong getCPUTicksPerSecond()
	{
		mach_timebase_info_data_t info;
		mach_timebase_info(&info);
		return 1000000000ull * info.denom / info.numer;
	}
}

#endif

namespace spadas
{
	class TimerVars : public Vars
	{
    public:
		SPADAS_VARS(Timer, Vars)

		ULong startCPUTick;

		TimerVars(ULong startCPUTick) : startCPUTick(startCPUTick)
		{}
	};
}

namespace timer_interval
{
	ULong cpuTicksPerSecondsConstant = 0;
	Double millisecondCoefficient = 0;

	inline void initTimerConstants()
	{
		if (cpuTicksPerSecondsConstant == 0)
		{
			cpuTicksPerSecondsConstant = getCPUTicksPerSecond();
			millisecondCoefficient = 1000.0 / cpuTicksPerSecondsConstant;
		}
	}
}

using namespace spadas;
using namespace timer_interval;

Timer::Timer() : Object<TimerVars>(new TimerVars(getCurrentCPUTick()), TRUE)
{}

Timer::Timer(ULong startCPUTick) : Object<TimerVars>(new TimerVars(math::min(getCurrentCPUTick(), startCPUTick)), TRUE)
{}

void Timer::start()
{
	vars->startCPUTick = getCurrentCPUTick();
}

Double Timer::check()
{
	initTimerConstants();
	return millisecondCoefficient * (getCurrentCPUTick() - vars->startCPUTick);
}

Double Timer::getStartTime()
{
	initTimerConstants();
	return millisecondCoefficient * vars->startCPUTick;
}

ULong Timer::getStartCPUTick()
{
	return vars->startCPUTick;
}

ULong Timer::cpuTick()
{
	return getCurrentCPUTick();
}

ULong Timer::cpuTicksPerSecond()
{
	initTimerConstants();
	return cpuTicksPerSecondsConstant;
}