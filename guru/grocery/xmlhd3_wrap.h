//#pragma once
//#ifndef _GURU_XMLHD3_WRAP_H_
//#define _GURU_XMLHD3_WRAP_H_
//
//#include "../gvals.h"
//#include "xmler.h"
//#include "xmlhd3.h"
//#include <memory>
//#include <type_traits>
//#include <cassert>
//
//_GURU_BEGIN
//
///*
//xml parser based xmlhd3
//*/
//class xmlhd3_wrap : public xmler<std::shared_ptr<XMLElement>, std::add_lvalue_reference_t<XMLElement>>
//{
//public:
//	typedef xmler<std::shared_ptr<XMLElement>, std::add_lvalue_reference_t<XMLElement>> base_type;
//	typedef std::shared_ptr<XMLElement> element_type;
//	typedef std::add_lvalue_reference_t<XMLElement> root_type;
//
//public:
//	xmlhd3_wrap(char const* file) noexcept
//		: _x{ file }
//	{
//		_ready = _x.ready();
//	}
//
//public:
//	root_type root() noexcept override
//	{
//		assert(_ready);
//		return _x.GetRootElement();
//	}
//
//	std::vector<std::shared_ptr<XMLElement>> all(char const* name) noexcept override
//	{
//		auto& childs = root().GetChildren();
//		std::vector<std::shared_ptr<XMLElement>> v;
//		for_each(childs.cbegin(), childs.cend(), [&](std::shared_ptr<XMLElement> e)
//		{
//			if (e->GetElementName() == name)
//				v.push_back(e);
//		});
//		return v;
//	}
//
//	std::vector<std::shared_ptr<XMLElement>> all(char const* name, char const* parent) noexcept override
//	{
//		return std::vector<std::shared_ptr<XMLElement>>();
//	}
//
//	std::vector<std::shared_ptr<XMLElement>> all(char const* name, char const* parent, char const* pparent) noexcept override
//	{
//		return std::vector<std::shared_ptr<XMLElement>>();
//	}
//
//private:
//	XML _x;
//};
//
//_GURU_END
//
//#endif /* _GURU_XMLHD3_WRAP_H_ */