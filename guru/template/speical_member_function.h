#pragma once
#include <utility>
#include <string>
#include <iostream>

class Person
{
private:
	std::string name;
public:
	template <typename STR>
	explicit Person(STR&& n) : name(std::forward<STR>(n))
	{
		std::cout << "TMPL_CONSTR for '" << name << "'\n";
	}

	Person(Person const& p) : name(p.name)
	{
		std::cout << "COPY_CONSTR Person '" << name << "'\n";
	}

	Person(Person&& p) : name(std::move(p.name))
	{
		std::cout << "MOVE_CONSTR Person '" << name << "'\n";
	}

	// really solution desc below
	template <
		typename STR,
		typename = std::enable_if_t<!std::is_same_v<Person, std::decay_t<STR>>>
		Person(STR&& n);				
};

void foo()
{
	std::string s = "sname";
	Person p1(s);	// init with string object => calls TMPL_CONSTR
	Person p2("tmp");	// init with string literal -> calls TMPL_CONSTR
	Person p3(p1);	// ERROR
	Person p4(std::move(p1));	// calls MOVE_CONSTR
	Person const p2c("ctmp");
	Person p3c(p2c);	// calls COPY_CONSTR
}

// according to the overload resolution rules of C++
// for a nonconstant lvalue Person p, the member template is a better match than copy ctr.
// define Person(Person& p) is not a perfect solution in inheriance
// you really need is to disable template when argument is a Person