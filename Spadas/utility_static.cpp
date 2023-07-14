
#include "spadas.h"
#include "oscillator.h"

#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

#if defined(SPADAS_ENV_WINDOWS)
#include <windows.h>
#endif

#define MEMOP_THRESH 16

namespace spadas_internal
{
	struct WindowsWaitContext
	{
		Atom sampleCount;
		Atom failCount;
	}
	wc;
}

using namespace spadas;
using namespace spadas_internal;

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
	return Environment::Linux;
#elif defined(SPADAS_ENV_WINDOWS)
	return Environment::Windows;
#elif defined(SPADAS_ENV_MACOS)
	return Environment::MacOS;
#else
	return Environment::Unknown;
#endif
}

// command
#if defined(SPADAS_ENV_WINDOWS)
void spadas::system::command(String cmd)
{
    STARTUPINFOW si;
    utility::memorySet(0, &si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFOW);

    PROCESS_INFORMATION pi;
    utility::memorySet(0, &pi, sizeof(PROCESS_INFORMATION));

    BOOL ret = CreateProcessW(NULL, cmd.wchars().data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    if (ret) WaitForSingleObject(pi.hProcess, UINF);
}
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
void spadas::system::command(String cmd)
{
	if (::system(cmd.chars().data())) {};
}
#endif

// memoryCopy
void spadas::utility::memoryCopy(const Pointer src, Pointer dst, UInt copySize)
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
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
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
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
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
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
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
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
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
	Array<String> srcComps = src.split(";");
	if (srcComps.contain(target)) return;

	String newVar = src + ";" + target;
	SetEnvironmentVariableW(L"Path", newVar.wchars().data());
}
#elif defined(SPADAS_ENV_LINUX)
void spadas::system::addEnvironmentPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull() || !path.isFolder());

	String targetFolder = path.fullPath();
	targetFolder = String(targetFolder, Region(0, targetFolder.length() - 1));

	String env = getenv("PATH");
	Array<String> envComps = env.split(":");
	if (!envComps.contain(targetFolder))
	{
		env += (String)":" + targetFolder;
		setenv("PATH", (Char*)env.bytes(), 1);
	}

	env = getenv("LD_LIBRARY_PATH");
	envComps = env.split(":");
	if (!envComps.contain(targetFolder))
	{
		env += (String)":" + targetFolder;
		setenv("LD_LIBRARY_PATH", (Char*)env.bytes(), 1);
	}
}
#elif defined(SPADAS_ENV_MACOS)
void spadas::system::addEnvironmentPath(Path path)
{
	SPADAS_ERROR_RETURN(path.isNull() || !path.isFolder());

	String targetFolder = path.fullPath();
	targetFolder = String(targetFolder, Region(0, targetFolder.length() - 1));

	String env = getenv("PATH");
	Array<String> envComps = env.split(":");
	if (!envComps.contain(targetFolder))
	{
		env += (String)":" + targetFolder;
		setenv("PATH", (Char*)env.bytes(), 1);
	}

	env = getenv("DYLD_LIBRARY_PATH");
	envComps = env.split(":");
	if (!envComps.contain(targetFolder))
	{
		env += (String)":" + targetFolder;
		setenv("DYLD_LIBRARY_PATH", (Char*)env.bytes(), 1);
	}
}
#endif
