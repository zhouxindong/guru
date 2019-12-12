#pragma once
#ifndef _GURU_NULL_FIXABLE_H_
#define _GURU_NULL_FIXABLE_H_

#include "../gvals.h"

_GURU_BEGIN

/*
** intrusive pre|suffix
** null operation
*/
struct null_fixable
{
	void prefix() noexcept {}
	void suffix() noexcept {}
};

_GURU_END

#endif /* _GURU_NULL_FIXABLE_H_ */
