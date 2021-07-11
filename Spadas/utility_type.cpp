
#include "spadas.h"

#include <time.h>

using namespace spadas;
using namespace spadas::math;

// Environment
Bool Environment::isValid(Int val)
{
    return val >= Environment::Unknown && val <= Environment::Windows;
}
String Environment::toString(Int val)
{
    switch (val)
    {
        case Environment::Unknown:
            return "Unknown";
        case Environment::Linux:
            return "Linux";
        case Environment::Windows:
            return "Windows";
        default:
			return String();
    }
}
Int Environment::defaultValue()
{
    return Environment::Unknown;
}

// Time
Time::Time()
{
	this->year = 1900;
	this->month = 1;
	this->day = 1;
	this->hour = 0;
	this->minute = 0;
	this->second = 0;
}

Time::Time(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second)
{
	this->year = year;
	this->month = month;
	this->day = day;
	this->hour = hour;
	this->minute = minute;
	this->second = second;
}

Bool Time::operator ==(Time time)
{
	return this->year == time.year &&
		this->month == time.month &&
		this->day == time.day &&
		this->hour == time.hour &&
		this->minute == time.minute &&
		this->second == time.second;
}

Bool Time::operator !=(Time time)
{
	return !operator ==(time);
}

String Time::dateString(String separator)
{
	return String(this->year, 4) + separator + String(this->month, 2) + separator + String(this->day, 2);
}

String Time::timeString(String separator)
{
	return String(this->hour, 2) + separator + String(this->minute, 2) + separator + String(this->second, 2);
}

String Time::toString(String dateSeparator, String timeSeparator, String dateTimeSeparator)
{
	return dateString(dateSeparator) + dateTimeSeparator + timeString(timeSeparator);
}

// TimeWithMS
Bool TimeWithMS::operator ==(TimeWithMS time)
{
	return dateTime == time.dateTime && milliseconds == time.milliseconds;
}

Bool TimeWithMS::operator !=(TimeWithMS time)
{
	return dateTime != time.dateTime || milliseconds != time.milliseconds;
}

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

void TimeWithMS::substract(TimeWithMS time, Int& week, Int& day, Int& hour, Int& minute, Int& second, Int& millisecond)
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

ULong TimeWithMS::localTimeToPosix()
{
	tm t;
	t.tm_year = dateTime.year - 1900;
	t.tm_mon = dateTime.month - 1;
	t.tm_mday = dateTime.day;
	t.tm_hour = dateTime.hour;
	t.tm_min = dateTime.minute;
	t.tm_sec = dateTime.second;
	time_t t1 = mktime(&t);
	return (ULong)(1000 * t1) + milliseconds;
}

namespace time_ms_internal
{
	Bool inited = FALSE;
	Long diff = 0;
}

ULong TimeWithMS::utcTimeToPosix()
{
	if (!time_ms_internal::inited)
	{
		TimeWithMS temp;
		temp.utcTimeFromPosix(1500000000000);
		time_ms_internal::diff = (Long)1500000000000 - (Long)temp.localTimeToPosix();
		time_ms_internal::inited = TRUE;
	}

	tm t;
	t.tm_year = dateTime.year - 1900;
	t.tm_mon = dateTime.month - 1;
	t.tm_mday = dateTime.day;
	t.tm_hour = dateTime.hour;
	t.tm_min = dateTime.minute;
	t.tm_sec = dateTime.second;
	time_t t1 = mktime(&t);
	return (ULong)(1000 * t1) + milliseconds + time_ms_internal::diff;
}

void TimeWithMS::localTimeFromPosix(ULong posixTime)
{
	time_t t1 = (time_t)(posixTime / 1000);
	tm t;
#if defined(SPADAS_ENV_WINDOWS)
	localtime_s(&t, &t1);
#else
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

void TimeWithMS::utcTimeFromPosix(ULong posixTime)
{
	time_t t1 = (time_t)(posixTime / 1000);
	tm t;
#if defined(SPADAS_ENV_WINDOWS)
	gmtime_s(&t, &t1);
#else
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

String TimeWithMS::toString(String dateSeparator, String timeSeparator, String dateTimeSeparator, String msSeparator)
{
	return dateTime.toString(dateSeparator, timeSeparator, dateTimeSeparator) + msSeparator + String(milliseconds, 3);
}