
#include "spadas.h"

#if defined(SPADAS_ENV_WINDOWS)

#include <Windows.h>

namespace spadas
{
	class TimerVars : public Vars
	{
    public:
		SPADAS_VARS_DEF(Timer, Vars)

		LARGE_INTEGER startTime;
		LARGE_INTEGER frequency;

		TimerVars()
		{
			startTime.QuadPart = 0;
			frequency.QuadPart = 0;
		}
	};
}

using namespace spadas;

const String spadas::Timer::TypeName = "spadas.Timer";

Timer::Timer() : Object<TimerVars>(new TimerVars(), TRUE)
{
	start();
}

void Timer::start()
{
	/* query frequency if not queried */
	if (vars->frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&vars->frequency);
	}

	/* query tickcount of start time */
	QueryPerformanceCounter(&vars->startTime);
}

Double Timer::check()
{
	/* query tickcount of current time */
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	/* calculate the passed time in millisecond */
	return (Double)(currentTime.QuadPart - vars->startTime.QuadPart) * 1000.0 / (Double)vars->frequency.QuadPart;
}

Double Timer::getStartTime()
{
	return (Double)(vars->startTime.QuadPart) * 1000.0 / (Double)vars->frequency.QuadPart;
}

#endif

#if defined(SPADAS_ENV_LINUX)

#include <sys/time.h>
#include <sys/select.h>

namespace spadas
{
	class TimerVars : public Vars
	{
	public:
		SPADAS_VARS_DEF(Timer, Vars)

		struct timeval startTime;
	};
}

using namespace spadas;

const String spadas::Timer::TypeName = "spadas.Timer";

Timer::Timer() : Object<TimerVars>(new TimerVars(), TRUE)
{
	start();
}

void Timer::start()
{
	gettimeofday(&vars->startTime, NULL);
}

Double Timer::check()
{
	struct timeval endTime;
	gettimeofday(&endTime, NULL);
	return (Double)(endTime.tv_sec - vars->startTime.tv_sec) * 1000 + (Double)(endTime.tv_usec - vars->startTime.tv_usec) / 1000;
}

Double Timer::getStartTime()
{
	return (Double)(vars->startTime.tv_sec) * 1000 + (Double)(vars->startTime.tv_usec) / 1000; 
}

#endif
