#ifndef _SIMPLETIME_H_
#define _SIMPLETIME_H_

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <time.h>

class SimpleTime
{
public:
	SimpleTime(): year(0), mon(0), day(0), hour(0), min(0), sec(0)
	{}
	SimpleTime(int y, int m, int d, int h, int mi, int s)
	:year(y), mon(m), day(d), hour(h), min(mi), sec(s)
	{}

public:
	static SimpleTime Now()
	{
		time_t t = time(NULL);
		tm* pt = localtime(&t);

		return SimpleTime(pt->tm_year+1900, pt->tm_mon+1, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
	}

	std::string TimeStamp()
	{
		return TimeFormat("%04d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
	}

	std::string Time14()
	{
		return TimeFormat("%04d%02d%02d%02d%02d%02d", year, mon, day, hour, min, sec);
	}

	std::string DayTime8()
	{
		return TimeFormat("%04d%02d%02d", year, mon, day);
	}

	std::string DayTime10()
	{
		return TimeFormat("%04d-%02d-%02d", year, mon, day);
	}

	std::string MonTime()
	{
		return TimeFormat("%04d%02d", year, mon);
	}

private:
	std::string TimeFormat(const char* format, ...)
	{
		if ( NULL == format )
		{
			return std::string();
		}

		char buf[32] = "";
		va_list arg_ptr;
		va_start(arg_ptr, format);
		vsprintf(buf, format, arg_ptr);
		va_end(arg_ptr);
		return buf;
	}

public:
	int year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
};

#endif

