
#include "spadas.h"

#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

#define MEMOP_THRESH 16

using namespace spadas;

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
#else
	return Environment::Unknown;
#endif
}

// command
void spadas::system::command(String cmd)
{
	if (::system(cmd.chars().data())) {};
}

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
#elif defined(SPADAS_ENV_LINUX)
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
#elif defined(SPADAS_ENV_LINUX)
Time spadas::system::getTime()
{
	time_t longTime;
	time(&longTime);
	tm *timeNow = localtime(&longTime);

	Time out;
	out.year = timeNow->tm_year + 1900;
	out.month = timeNow->tm_mon + 1;
	out.day = timeNow->tm_mday;
	out.hour = timeNow->tm_hour;
	out.minute = timeNow->tm_min;
	out.second = timeNow->tm_sec;

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
#elif defined(SPADAS_ENV_LINUX)
TimeWithMS spadas::system::getTimeWithMS()
{
	timeb nowTime;
	ftime(&nowTime);

	tm *timeNow = localtime(&nowTime.time);

	TimeWithMS out;
	out.dateTime.year = timeNow->tm_year + 1900;
	out.dateTime.month = timeNow->tm_mon + 1;
	out.dateTime.day = timeNow->tm_mday;
	out.dateTime.hour = timeNow->tm_hour;
	out.dateTime.minute = timeNow->tm_min;
	out.dateTime.second = timeNow->tm_sec;
	out.milliseconds = nowTime.millitm;

	return out;
}
#endif

// addEnvironmentPath
#if defined(SPADAS_ENV_WINDOWS)
#include <Windows.h>
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
#endif
