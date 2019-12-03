#pragma once
#ifndef _GURU_COUNTABLE_H_
#define _GURU_COUNTABLE_H_

#include "../gvals.h"

_GURU_BEGIN

/*
** CRTP
** 1. 继承自模板类
** 2. 使用派生类作为模板参数特化基类
*/
template <typename _T>
class __base
{
public:
	/*
	** 在基类中使用派生类
	*/
	void do_what()
	{
		_T& derived = static_cast<_T&>(*this);
	}

private:
	// 只允许CRTP构造，即基类的模板参数实参必须是派生类型
	__base() {};
	friend _T;
};

/*
** 派生类会隐藏基类同名方法
*/
class __derived : public __base<__derived>
{};

/*
** 应用场合
** 1. 扩展派生类的接口(利用派生类的接口语义定义新接口)
*/
template <typename _T>
struct __expression
{
	_T const& cast() const
	{
		return static_cast<_T const&>(*this);
	}

	_T& cast()
	{
		return static_cast<_T&>(*this);
	}

	void new_api()
	{
		case().old_api();
	}
};

struct __square : public __expression<__square>
{
	void old_api() {}
};

/*
** countable
*/
template <typename CountedType>
class countable
{
private:
	static size_t _count;

protected:
	countable()
	{
		++countable<CountedType>::_count;
	}

	countable(countable<CountedType> const&)
	{
		++countable<CountedType>::_count;
	}

	~countable()
	{
		--countable<CountedType>::_count;
	}

public:
	static
	size_t
	live()
	{
		return countable<CountedType>::_count;
	}
};

template <typename CountedType>
size_t countable<CountedType>::_count = 0;

/*
** CRTP: Curiously Recurring Template Pattern
** 通常CRTP适用于仅能用作成员函数的接口（如构造函数、析构函数和[]等）的实现提取出来
*/
class nontempl_class : public countable<nontempl_class>
{
};

template <typename _Tx>
class templ_class : public countable<templ_class<_Tx>>
{
};

_GURU_END

#endif /* _GURU_COUNTABLE_H_ */