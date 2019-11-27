#pragma once
#ifndef _GURU_CHRONO_EX_H_
#define _GURU_CHRONO_EX_H_

#include "../gvals.h"
#include <cstdint>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <tuple>
#include "low_chrono.h"

_GURU_BEGIN

static constexpr long UTC_BIAS = 8;

typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>> days;

/*
** 0000-01-01
*/
constexpr 
inline
days proleptic_gregorian_epoch() noexcept
{
	return days(days_from_civil(0, 1, 1));
}

/*
** get year-mon-day-hour-min-sec-ms from a time_point
** utc == false returns a local time, or returns a UTC time
*/
inline
std::tuple<unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned>
from_time_point(std::chrono::system_clock::time_point const& tp, bool utc = false) noexcept
{
	using namespace std;
	using namespace std::chrono;
	auto accu = utc ? tp.time_since_epoch() : tp.time_since_epoch() + hours(UTC_BIAS);
	auto delta = accu - proleptic_gregorian_epoch();

	auto day = duration_cast<days>(accu);
	unsigned y; 
	unsigned m;
	unsigned d;
	tie(y, m, d) = civil_from_days(day.count()); 

	days dt = duration_cast<days>(delta);
	delta -= dt;
	auto hour = duration_cast<hours>(delta);
	delta -= hour;
	auto min = duration_cast<minutes>(delta);
	delta -= min;
	auto sec = duration_cast<seconds>(delta);
	delta -= sec;
	auto ms = duration_cast<milliseconds>(delta);

	return make_tuple(y, m, d, (unsigned)hour.count(), (unsigned)min.count(), 
		(unsigned)sec.count(), (unsigned)ms.count());
}

/*
** format a time_point value to string
*/
inline 
std::string time_point_to_string(std::chrono::system_clock::time_point const& tp, bool utc = false) noexcept
{
	auto tu = from_time_point(tp, utc);

	std::ostringstream stream;
	stream << std::get<0>(tu) << "-";
	stream << std::setw(2) << std::setfill('0') << std::get<1>(tu) << '-';
	stream << std::setw(2) << std::setfill('0') << std::get<2>(tu) << ' ';
	stream << std::setw(2) << std::setfill('0') << std::get<3>(tu) << ':';
	stream << std::setw(2) << std::setfill('0') << std::get<4>(tu) << ':';
	stream << std::setw(2) << std::setfill('0') << std::get<5>(tu) << '.';
	stream << std::setw(3) << std::setfill('0') << std::get<6>(tu);

	return stream.str();
}

/*
** get time_point by y-m-d-h-m-s-ms
*/
inline 
std::chrono::system_clock::time_point
to_time_point(
	unsigned year, unsigned mon, unsigned day, 
	unsigned hour = 0, unsigned min = 0, unsigned sec = 0, 
	unsigned ms = 0, bool utc = false)
{
	auto tp = std::chrono::system_clock::time_point(
		days(days_from_civil(year, mon, day)) +
		std::chrono::hours(hour) +
		std::chrono::minutes(min) +
		std::chrono::seconds(sec) +
		std::chrono::milliseconds(ms)
	);
	if (!utc) tp -= std::chrono::hours(UTC_BIAS);
	return tp;
}

inline long long elapse_sec(std::chrono::system_clock::time_point const& begin)
{
	return std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::system_clock::now() - begin).count();
}

inline long long elapse_ms(std::chrono::system_clock::time_point const& begin)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - begin).count();
}

inline long long now_to_sec()
{
	return std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
}

inline long long now_to_ms()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
}

inline long long time_point_to_sec(std::chrono::system_clock::time_point const& tp)
{
	return std::chrono::duration_cast<std::chrono::seconds>
		(tp.time_since_epoch()).count();
}

inline long long time_point_to_ms(std::chrono::system_clock::time_point const& tp)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(tp.time_since_epoch()).count();
}

_GURU_END

#endif /* _GURU_CHRONO_EX_H_ */