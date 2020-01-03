#pragma once
#ifndef _GURU_XMLER_H_
#define _GURU_XMLER_H_

#include "../gvals.h"
#include <vector>
#include <utility>

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
	/** build xml tree from a string
		usually after default ctor		
	*/
	virtual bool parse(char const* xml) = 0;

	/** retrive all element with speical name
		@verbatim
		<name>
		@endverbatim
	*/
	virtual std::vector<_Vec_ele> all(char const* name) = 0;

	/** retrive all elements with special name and its parent
		@verbatim
		<parent>
			<name>
		@endverbatim
	*/
	virtual std::vector<_Vec_ele> all(char const* name, char const* parent) = 0;

	/** retrive all elements with special name and its parent and grand-parent
		@verbatim
		<grand-parent>
			<parent>
				<name>
		@endverbatim
	*/
	virtual std::vector<_Vec_ele> all(char const* name, char const* parent, char const* pparent) = 0;
	
	/** return the root elements
	*/
	virtual _Root_ele root() = 0;

	/** return all the directly children for special element
		parameter name can point child with special name
	*/
	virtual std::vector<_Vec_ele> childs(_Vec_ele ele, char const* name = (const char*)0) = 0;

public:
	virtual std::pair<bool, int32_t> int32_attribute(char const* name) = 0;


public:
	virtual char const* text(_Vec_ele ele) = 0;

public:
	bool ready() const noexcept { return _ready; }

protected:
	bool _ready{ false };
};

_GURU_END

#endif /* _GURU_XMLER_H_ */