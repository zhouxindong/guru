#pragma once
#ifndef _GURU_TINYXML2_WRAP_H_
#define _GURU_TINYXML2_WRAP_H_

#include "../gvals.h"
#include "xmler.h"
#include "tinyxml2.h"
#include <cassert>

_GURU_BEGIN

/*
 wrap class for tinyxml2
*/
class tinyxml2_wrap : public xmler<guru::XMLElement*>
{
public:
	tinyxml2_wrap() {}

	tinyxml2_wrap(char const* file)
	{
		_ready = (guru::XML_SUCCESS == _doc.LoadFile(file));
	}

	bool parse(char const* xml) override
	{
		_ready = (guru::XML_SUCCESS == _doc.Parse(xml));
		return _ready;
	}

	tinyxml2_wrap(tinyxml2_wrap const&) = delete;
	tinyxml2_wrap(tinyxml2_wrap&&) = delete;
	tinyxml2_wrap& operator=(tinyxml2_wrap const&) = delete;
	tinyxml2_wrap& operator=(tinyxml2_wrap&&) = delete;

public:
	root_type root() override
	{
		assert(_ready);
		return _doc.RootElement();
	}

	std::vector<element_type> all(const char* name) override
	{
		assert(_ready);
		_vec.clear();
		__all(name, root());
		return _vec;
	}

	std::vector<element_type> all(const char* name, const char* parent) override
	{
		assert(_ready);
		_vec.clear();
		__all(name, root(), parent);
		return _vec;
	}

	std::vector<element_type> all(const char* name, const char* parent, const char* pparent) override
	{
		assert(_ready);
		_vec.clear();
		__all(name, root(), parent, pparent);
		return _vec;
	}

	std::vector<element_type> childs(element_type ele, char const* name = (char const*)0)
	{
		assert(_ready);
		std::vector<element_type> v;
		if (ele->NoChildren())
			return v;

		element_type n = ele->FirstChildElement(name);
		while (n)
		{
			v.push_back(n);
			n = n->NextSiblingElement(name);
		}
		return v;
	}

private:
	void __all(const char* name, element_type ele)
	{
		if (::strcmp(name, ele->Name()) == 0) {
			_vec.push_back(ele);
		}
		if (ele->FirstChildElement()) {
			__all(name, ele->FirstChildElement());
		}
		if (ele->NextSiblingElement()) {
			__all(name, ele->NextSiblingElement());
		}
	}

	void __all(const char* name, element_type ele, const char* parent) 
	{
		if (::strcmp(name, ele->Name()) == 0) {
			if (ele->Parent() &&
				(::strcmp(parent, ele->Parent()->ToElement()->Name()) == 0))
				_vec.push_back(ele);
		}
		if (ele->FirstChildElement()) {
			__all(name, ele->FirstChildElement(), parent);
		}
		if (ele->NextSiblingElement()) {
			__all(name, ele->NextSiblingElement(), parent);
		}
	}

	void __all(const char* name, element_type ele, const char* parent, const char* pparent)
	{
		if (::strcmp(name, ele->Name()) == 0) {
			if (ele->Parent() &&
				(::strcmp(parent, ele->Parent()->ToElement()->Name()) == 0))
				if (ele->Parent()->Parent() &&
					(::strcmp(pparent, ele->Parent()->Parent()->ToElement()->Name()) == 0))
					_vec.push_back(ele);
		}
		if (ele->FirstChildElement()) {
			__all(name, ele->FirstChildElement(), parent, pparent);
		}
		if (ele->NextSiblingElement()) {
			__all(name, ele->NextSiblingElement(), parent, pparent);
		}
	}

private:
	guru::XMLDocument _doc;
	std::vector<element_type> _vec;
};

_GURU_END

#endif /* _GURU_TINYXML2_WRAP_H_ */