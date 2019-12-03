#pragma once
#ifndef _GURU_NON_COPYABLE_H_
#define _GURU_NON_COPYABLE_H_

#include "../gvals.h"
#include <tuple>

_GURU_BEGIN

/*
** append non copy function to class
** usage: non_copyable<class>
*/
template <class _Base>
struct non_copyable : public _Base
{
	explicit non_copyable() = default;

	struct non_copyable_exception {};

	template <typename... _Args>
	explicit non_copyable(_Args&&... args)
		: _Base(std::forward<_Args>(args)...) 
	{
		auto t = std::tuple<_Args&&...>(std::forward<_Args>(args)...);
		if (std::tuple_size<decltype(t)>::value != 1)
			return;

		if (typeid(std::tuple_element<0, decltype(t)>::type).hash_code() ==
			typeid(decltype(*this)).hash_code()) 
		{
			throw non_copyable_exception();
		}
	};

	non_copyable(const non_copyable&) = delete;
	non_copyable& operator = (const non_copyable&) = delete;
};


_GURU_END

#endif /* _GURU_NON_COPYABLE_H_ */