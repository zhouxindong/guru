#pragma once
#ifndef _GURU_GROCERY_X64FMT_H_
#define _GURU_GROCERY_X64FMT_H_

// printf macros for size_t, in the style of inttypes.h
#ifdef _LP64
#define __PRIS_PREFIX "z"
#else
#define __PRIS_PREFIX
#endif

/** Use these macros after a % in a printf format string
	to get correct 32/64 bit behavior, like this:
	size_t size = records.size();
	printf("%"PRIuS"\n", size);
*/
#define PRIdS __PRIS_PREFIX "d"
#define PRIxS __PRIS_PREFIX "x"
#define PRIuS __PRIS_PREFIX "u"
#define PRIXS __PRIS_PREFIX "X"
#define PRIoS __PRIS_PREFIX "o"

/** type			don't use			use						memo
	void*(other*)	%lx					%p
	int64_t			%qd, %lld			%"PRId64"
	uint64_t		%qu, %llu, %llx		%"PRIu64", %"PRIx64"
	size_t			%u					%"PRIuS", %"PRIxS"		C99 special %zu
	ptrdiff_t		%d					%"PRIdS"				C99 special %zd
*/

#include "../gvals.h"



_GURU_BEGIN



_GURU_END

#endif // _GURU_GROCERY_X64FMT_H_