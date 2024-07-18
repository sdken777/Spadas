
#include <time.h>
#include "spadas.h"

using namespace spadas;
using namespace spadas::math;

// Time
Bool Time::operator >(Time time)
{
	ULong t1 = ((ULong)this->year << 40) + ((ULong)this->month << 32) + ((ULong)this->day << 24) + 
		((ULong)this->hour << 16) + ((ULong)this->minute << 8) + (ULong)this->second;
	ULong t2 = ((ULong)time.year << 40) + ((ULong)time.month << 32) + ((ULong)time.day << 24) + 
		((ULong)time.hour << 16) + ((ULong)time.minute << 8) + (ULong)time.second;
	return t1 > t2;
}

Bool Time::operator <(Time time)
{
	ULong t1 = ((ULong)this->year << 40) + ((ULong)this->month << 32) + ((ULong)this->day << 24) + 
		((ULong)this->hour << 16) + ((ULong)this->minute << 8) + (ULong)this->second;
	ULong t2 = ((ULong)time.year << 40) + ((ULong)time.month << 32) + ((ULong)time.day << 24) + 
		((ULong)time.hour << 16) + ((ULong)time.minute << 8) + (ULong)time.second;
	return t1 < t2;
}

// TimeWithMS
TimeWithMS TimeWithMS::operator +(Long millisecond)
{
	ULong posixTime = localTimeToPosix();
	posixTime += millisecond;
	TimeWithMS output;
	output.localTimeFromPosix(posixTime);
	return output;
}

Long TimeWithMS::operator -(TimeWithMS time)
{
	Long posixTime1 = (Long)localTimeToPosix();
	Long posixTime2 = (Long)time.localTimeToPosix();
	return posixTime1 - posixTime2;
}

void TimeWithMS::minus(TimeWithMS time, Int& week, Int& day, Int& hour, Int& minute, Int& second, Int& millisecond)
{
	Long posixTime1 = (Long)localTimeToPosix();
	Long posixTime2 = (Long)time.localTimeToPosix();
	Long diff = posixTime1 - posixTime2;
	Int positive = diff >= 0 ? 1 : -1;
	diff = math::abs(diff);
	week = (Int)(positive * (diff / 604800000));
	day = positive * ((diff / 86400000) % 7);
	hour = positive * ((diff / 3600000) % 24);
	minute = positive * ((diff / 60000) % 60);
	second = positive * ((diff / 1000) % 60);
	millisecond = positive * (diff % 1000);
}

MilliPosix TimeWithMS::localTimeToPosix()
{
	tm t;
	utility::memorySet(0, &t, sizeof(t));
	t.tm_year = dateTime.year - 1900;
	t.tm_mon = dateTime.month - 1;
	t.tm_mday = dateTime.day;
	t.tm_hour = dateTime.hour;
	t.tm_min = dateTime.minute;
	t.tm_sec = dateTime.second;
	t.tm_isdst = -1;
	time_t t1 = mktime(&t);
	return (ULong)t1 * 1000 + milliseconds;
}

namespace time_ms_internal
{
	Bool inited = FALSE;
	Long diff = 0;
}

MilliPosix TimeWithMS::utcTimeToPosix()
{
	if (!time_ms_internal::inited)
	{
		TimeWithMS temp;
		temp.utcTimeFromPosix(1500000000000);
		time_ms_internal::diff = (Long)1500000000000 - (Long)temp.localTimeToPosix();
		time_ms_internal::inited = TRUE;
	}

	tm t;
	utility::memorySet(0, &t, sizeof(t));
	t.tm_year = dateTime.year - 1900;
	t.tm_mon = dateTime.month - 1;
	t.tm_mday = dateTime.day;
	t.tm_hour = dateTime.hour;
	t.tm_min = dateTime.minute;
	t.tm_sec = dateTime.second;
	t.tm_isdst = -1;
	time_t t1 = mktime(&t);
	return (ULong)t1 * 1000 + milliseconds + time_ms_internal::diff;
}

void TimeWithMS::localTimeFromPosix(MilliPosix posixTime)
{
	time_t t1 = (time_t)(posixTime / 1000);
	tm t;
#if defined(SPADAS_ENV_WINDOWS)
	localtime_s(&t, &t1);
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
	localtime_r(&t1, &t);
#endif

	dateTime.year = 1900 + t.tm_year;
	dateTime.month = 1 + t.tm_mon;
	dateTime.day = t.tm_mday;
	dateTime.hour = t.tm_hour;
	dateTime.minute = t.tm_min;
	dateTime.second = t.tm_sec;
	milliseconds = posixTime % 1000;
}

void TimeWithMS::utcTimeFromPosix(MilliPosix posixTime)
{
	time_t t1 = (time_t)(posixTime / 1000);
	tm t;
#if defined(SPADAS_ENV_WINDOWS)
	gmtime_s(&t, &t1);
#elif defined(SPADAS_ENV_LINUX) || defined(SPADAS_ENV_MACOS)
	gmtime_r(&t1, &t);
#endif

	dateTime.year = 1900 + t.tm_year;
	dateTime.month = 1 + t.tm_mon;
	dateTime.day = t.tm_mday;
	dateTime.hour = t.tm_hour;
	dateTime.minute = t.tm_min;
	dateTime.second = t.tm_sec;
	milliseconds = posixTime % 1000;
}