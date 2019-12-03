#pragma once
#ifndef _GURU_COUNTABLE_H_
#define _GURU_COUNTABLE_H_

#include "../gvals.h"

_GURU_BEGIN

/*
** CRTP
** 1. �̳���ģ����
** 2. ʹ����������Ϊģ������ػ�����
*/
template <typename _T>
class __base
{
public:
	/*
	** �ڻ�����ʹ��������
	*/
	void do_what()
	{
		_T& derived = static_cast<_T&>(*this);
	}

private:
	// ֻ����CRTP���죬�������ģ�����ʵ�α�������������
	__base() {};
	friend _T;
};

/*
** ����������ػ���ͬ������
*/
class __derived : public __base<__derived>
{};

/*
** Ӧ�ó���
** 1. ��չ������Ľӿ�(����������Ľӿ����嶨���½ӿ�)
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
** ͨ��CRTP�����ڽ���������Ա�����Ľӿڣ��繹�캯��������������[]�ȣ���ʵ����ȡ����
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