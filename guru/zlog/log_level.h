#pragma once
#ifndef _GURU_LOG_LEVEL_H_
#define _GURU_LOG_LEVEL_H_

#include "../gvals.h"

_GURU_BEGIN

enum _LOG_LEVEL
{
	_LOG_TRACE = 10155,
	_LOG_DEBUG,
	_LOG_INFO,
	_LOG_WARN,
	_LOG_ERROR,
	_LOG_FATAL
};

constexpr
inline
const char* 
log_level(_LOG_LEVEL level) noexcept
{
	switch (level)
	{
	case _LOG_TRACE:
		return "TRACE";

	case _LOG_DEBUG:
		return "DEBUG";

	case _LOG_INFO:
		return "INFO";

	case _LOG_WARN:
		return "WARN";

	case _LOG_ERROR:
		return "ERROR";

	case _LOG_FATAL:
		return "FATAL";

	default:
		return "UNKNOWN";
	}
}

_GURU_END

#endif /* _GURU_LOG_LEVEL_H_ */