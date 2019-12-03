#pragma once
#ifndef _GURU_PREFIXABLE_H_
#define _GURU_PREFIXABLE_H_

#include "../gvals.h"

_GURU_BEGIN

/*
** append prefix call to class
** usage: prefixable<class, prefix_function_object>
*/
template <class _Base, class _Prefix>
struct prefixable : public _Base 
{
	template<typename... _Args>
	explicit prefixable(_Prefix* pp, _Args&&... args)
		: _Base(std::forward<_Args>(args)...), _prefix(pp) {}

	//template<typename... _Args>
	_Base* operator -> (/*_Args&&... args*/) 
	{
		(*_prefix)(/*std::forward<_Args>(args)...*/);
		return this;
	}

private:
	_Prefix* _prefix;
};


_GURU_END

#endif /* _GURU_PREFIXABLE_H_ */