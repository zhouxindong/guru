#pragma once
#ifndef _GURU_NON_INSTANTABLE_H_
#define _GURU_NON_INSTANTABLE_H_

#include "../gvals.h"

_GURU_BEGIN

/*
** append non instant function to class
** usage: non_instantable<class>
*/
template <class _Base>
struct non_instantable : public _Base
{
	non_instantable() = delete;

	//template <typename... _Args>
	//explicit non_instantable(_Args&&... args)
	//	: _Base(std::forward<_Args>(args)...) = delete;
};

_GURU_END

#endif /* _GURU_NON_INSTANTABLE_H_ */