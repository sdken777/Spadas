
#include "oscillator.h"
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

#define MEMOP_THRESH 16

#if !defined(SPADAS_ENV_NILRT)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

namespace utility_internal
{
	using namespace spadas;
	
	struct WindowsWaitContext
	{
		Atom sampleCount;
		Atom failCount;
	}
	wc;
}


using namespace spadas;
using namespace oscillator_internal;
using namespace utility_internal;

// isBigEndian
Bool spadas::system::isBigEndian()
{
	Word a = 255;
	Byte *aPtr = (Byte*)&a;
	return *aPtr != 255;
}

// exit
void spadas::system::exit()
{
	::exit(0);
}

// memorySet
void spadas::utility::memorySet(Byte value, Pointer dst, UInt setSize)
{
	if (setSize < MEMOP_THRESH)
	{
		Byte* dstBytes = (Byte*)dst;
		for (UInt i = 0; i < setSize; i++) dstBytes[i] = value;
	}
	else
	{
		memset(dst, (int)value, (size_t)setSize);
	}
}

// getEnv
Enum<Environment> spadas::system::getEnv()
{
#if defined(SPADAS_ENV_LINUX)
	return Environment::Value::Linux;
#elif defined(SPADAS_ENV_WINDOWS)
	return Environment::Value::Windows;
#elif defined(SPADAS_ENV_MACOS)
	return Environment::Value::MacOS;
#elif defined(SPADAS_ENV_NILRT)
	return Environment::Value::NILRT;
#else
	return Environment::Value::Unknown;
#endif
}

// command
void spadas::system::command(String cmd)
{
	if (::system(cmd.chars().data())) {};
}

// memoryCopy
void spadas::utility::memoryCopy(ConstPointer src, Pointer dst, UInt copySize)
{
	if (copySize < MEMOP_THRESH)
	{
		const Byte* srcBytes = (const Byte*)src;
		Byte* dstBytes = (Byte*)dst;
		for (UInt i = 0; i < copySize; i++) dstBytes[i] = srcBytes[i];
	}
	else
	{
#if defined(SPADAS_ENV_WINDOWS)
		memcpy_s(dst, (rsize_t)copySize, src, (rsize_t)copySize);
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)
		memcpy(dst, src, (size_t)copySize);
#endif
	}
}

// getTime
#if defined(SPADAS_ENV_WINDOWS)
Time spadas::system::getTime()
{
	tm timeNow;
	time_t longTime;
	time(&longTime);
	localtime_s(&timeNow, &longTime);

	Time out;
	out.year = timeNow.tm_year + 1900;
	out.month = timeNow.tm_mon + 1;
	out.day = timeNow.tm_mday;
	out.hour = timeNow.tm_hour;
	out.minute = timeNow.tm_min;
	out.second = timeNow.tm_sec;

	return out;
}
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)
Time spadas::system::getTime()
{
	time_t longTime;
	time(&longTime);

	tm timeNow;
	localtime_r(&longTime, &timeNow);

	Time out;
	out.year = timeNow.tm_year + 1900;
	out.month = timeNow.tm_mon + 1;
	out.day = timeNow.tm_mday;
	out.hour = timeNow.tm_hour;
	out.minute = timeNow.tm_min;
	out.second = timeNow.tm_sec;

	return out;
}
#endif

// getTimeWithMS
#if defined(SPADAS_ENV_WINDOWS)
TimeWithMS spadas::system::getTimeWithMS()
{
	timeb nowTime;
	ftime(&nowTime);

	tm timeNow;
	localtime_s(&timeNow, &nowTime.time);

	TimeWithMS out;
	out.dateTime.year = timeNow.tm_year + 1900;
	out.dateTime.month = timeNow.tm_mon + 1;
	out.dateTime.day = timeNow.tm_mday;
	out.dateTime.hour = timeNow.tm_hour;
	out.dateTime.minute = timeNow.tm_min;
	out.dateTime.second = timeNow.tm_sec;
	out.milliseconds = nowTime.millitm;

	return out;
}
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)
TimeWithMS spadas::system::getTimeWithMS()
{
	timeb nowTime;
	ftime(&nowTime);

	tm timeNow;
	localtime_r(&nowTime.time, &timeNow);

	TimeWithMS out;
	out.dateTime.year = timeNow.tm_year + 1900;
	out.dateTime.month = timeNow.tm_mon + 1;
	out.dateTime.day = timeNow.tm_mday;
	out.dateTime.hour = timeNow.tm_hour;
	out.dateTime.minute = timeNow.tm_min;
	out.dateTime.second = timeNow.tm_sec;
	out.milliseconds = nowTime.millitm;

	return out;
}
#endif

// wait
void spadas::system::wait(UInt time)
{
	if (time == 0) return;
#if defined(SPADAS_ENV_WINDOWS)
	if (wc.failCount.get() > 1) Flag(time).waitSet();
	else if (wc.sampleCount.get() >= 100) sleepTime(time);
	else
	{
		Timer timer;
		sleepTime(1);
		Bool failed = timer.check() >= 10.0;
		if (wc.sampleCount.increase() <= 100 && failed) wc.failCount.increase();
		sleepTime(time);
	}
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS) || defined(SPADAS_ENV_NILRT)
	sleepTime(time);
#endif
}

// waitSpin
void spadas::system::waitSpin(UInt timeUS)
{
	if (timeUS == 0) return;
	ULong target = Timer::cpuTick() + Timer::cpuTicksPerSecond() * timeUS / 1000000;
	while (Timer::cpuTick() < target) {}
}

// addEnvironmentPath
#if defined(SPADAS_ENV_WINDOWS)
#include <windows.h>
void spadas::system::addEnvironmentPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull() || !path.isFolder());

	WChar buffer[4096];
	GetEnvironmentVariableW(L"Path", buffer, 4096);
	String src(buffer);

	String target = path.fullPath();
	Array<StringSpan> srcComps = src.split(";");
	if (srcComps.containAs([target](StringSpan& span){ return span == target; })) return;

	String newVar = src + ";" + target;
	SetEnvironmentVariableW(L"Path", newVar.wchars().data());
}
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_NILRT)
void spadas::system::addEnvironmentPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull() || !path.isFolder());

	String targetFolder = path.fullPath();
	targetFolder = targetFolder.subString(0, targetFolder.length() - 1).clone();

	auto k = [targetFolder](StringSpan comp){ return comp == targetFolder; };

	String env = getenv("PATH");
	Array<StringSpan> envComps = env.split(":");

	if (!envComps.containAs([targetFolder](StringSpan& span){ return span == targetFolder; }))
	{
		env += (String)":" + targetFolder;
		setenv("PATH", env.chars().data(), 1);
	}

	env = getenv("LD_LIBRARY_PATH");
	envComps = env.split(":");
	if (!envComps.containAs([targetFolder](StringSpan& span){ return span == targetFolder; }))
	{
		env += (String)":" + targetFolder;
		setenv("LD_LIBRARY_PATH", env.chars().data(), 1);
	}
}
#elif defined(SPADAS_ENV_MACOS)
void spadas::system::addEnvironmentPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull() || !path.isFolder());

	String targetFolder = path.fullPath();
	targetFolder = targetFolder.subString(0, targetFolder.length() - 1).clone();

	String env = getenv("PATH");
	Array<StringSpan> envComps = env.split(":");
	if (!envComps.containAs([targetFolder](StringSpan& span){ return span == targetFolder; }))
	{
		env += (String)":" + targetFolder;
		setenv("PATH", env.chars().data(), 1);
	}

	env = getenv("DYLD_LIBRARY_PATH");
	envComps = env.split(":");
	if (!envComps.containAs([targetFolder](StringSpan& span){ return span == targetFolder; }))
	{
		env += (String)":" + targetFolder;
		setenv("DYLD_LIBRARY_PATH", env.chars().data(), 1);
	}
}
#endif
