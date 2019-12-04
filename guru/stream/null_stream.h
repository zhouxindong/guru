#pragma once
#ifndef _GURU_NULL_STREAM_H_
#define _GURU_NULL_STREAM_H_

#include "../gvals.h"
#include <ostream>

_GURU_BEGIN

/*
** null_stream
** a dummy stream, swallow anything
*/
class null_stream : public std::ostream 
{
public:
	null_stream() noexcept : std::ostream(0) 
	{
	}
};


_GURU_END

#endif /* _GURU_NULL_STREAM_H_ */