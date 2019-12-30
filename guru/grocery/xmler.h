#pragma once
#ifndef _GURU_XMLER_H_
#define _GURU_XMLER_H_

#include "../gvals.h"
#include <vector>

_GURU_BEGIN

/*
xml parser interface
*/
template <typename _Vec_ele, typename _Root_ele = _Vec_ele>
class xmler
{
public:
	typedef xmler<_Vec_ele> type;
	typedef _Vec_ele element_type;
	typedef _Root_ele root_type;

public:
	virtual ~xmler() noexcept {}

public:
	virtual bool parse(char const* xml) = 0;

	virtual std::vector<_Vec_ele> all(char const* name) = 0;
	virtual std::vector<_Vec_ele> all(char const* name, char const* parent) = 0;
	virtual std::vector<_Vec_ele> all(char const* name, char const* parent, char const* pparent) = 0;
	
	virtual _Root_ele root() = 0;

	virtual std::vector<_Vec_ele> childs(_Vec_ele ele, char const* name = (const char*)0) = 0;
public:
	bool ready() const noexcept { return _ready; }

protected:
	bool _ready{ false };
};

_GURU_END

#endif /* _GURU_XMLER_H_ */