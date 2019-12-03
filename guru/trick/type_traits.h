#pragma once
#ifndef _GURU_TYPE_TRAITS_H_
#define _GURU_TYPE_TRAITS_H_

#include "../gvals.h"

_GURU_BEGIN

template <typename _Tx>
class is_funda_type
{
public:
	static constexpr bool value = false;
	typedef is_funda_type<_Tx> type;
};

#define MK_IS_FUNDA_TYPE(T)					\
	template<>								\
	class is_funda_type<T>					\
	{										\
	public:									\
		static constexpr bool value = true;	\
		typedef is_funda_type<T> type;		\
	};										\

MK_IS_FUNDA_TYPE(void)
MK_IS_FUNDA_TYPE(bool)
MK_IS_FUNDA_TYPE(char)
MK_IS_FUNDA_TYPE(signed char)
MK_IS_FUNDA_TYPE(unsigned char)
MK_IS_FUNDA_TYPE(wchar_t)

MK_IS_FUNDA_TYPE(signed short)
MK_IS_FUNDA_TYPE(unsigned short)
MK_IS_FUNDA_TYPE(signed int)
MK_IS_FUNDA_TYPE(unsigned int)
MK_IS_FUNDA_TYPE(signed long)
MK_IS_FUNDA_TYPE(unsigned long)

#if LONGLONG_EXISTS
	MK_IS_FUNDA_TYPE(signed long long)
	MK_IS_FUNDA_TYPE(unsigned long long)
#endif

MK_IS_FUNDA_TYPE(float)
MK_IS_FUNDA_TYPE(double)
MK_IS_FUNDA_TYPE(long double)

#undef MK_IS_FUNDA_TYPE

template <typename _Tx>
class compound_type
{
public:
	static constexpr bool is_ptr = false;
	static constexpr bool is_ref = false;
	static constexpr bool is_array = false;
	static constexpr bool if_func = false;
	static constexpr bool is_ptr_mem = false;

	typedef _Tx base_type;
	typedef _Tx bottom_type;
	typedef compound_type<void> class_type;
};

template <typename _Tx>
class compound_type<_Tx&>
{
public:
	static constexpr bool is_ptr = false;
	static constexpr bool is_ref = true;
	static constexpr bool is_array = false;
	static constexpr bool if_func = false;
	static constexpr bool is_ptr_mem = false;

	typedef _Tx& base_type;
	typedef typename compound_type<_Tx>::bottom_type bottom_type;
	typedef compound_type<void> class_type;
};

template <typename _Tx>
class compound_type<_Tx*>
{
public:
	static constexpr bool is_ptr = true;
	static constexpr bool is_ref = false;
	static constexpr bool is_array = false;
	static constexpr bool if_func = false;
	static constexpr bool is_ptr_mem = false;

	typedef _Tx* base_type;
	typedef typename compound_type<_Tx>::bottom_type bottom_type;
	typedef compound_type<void> class_type;
};

template <typename _Tx, size_t _N>
class compound_type<_Tx[_N]>
{
public:
	static constexpr bool is_ptr = false;
	static constexpr bool is_ref = false;
	static constexpr bool is_array = true;
	static constexpr bool if_func = false;
	static constexpr bool is_ptr_mem = false;

	typedef _Tx(base_type)[_N];
	typedef typename compound_type<_Tx>::bottom_type bottom_type;
	typedef compound_type<void> class_type;
};

template <typename _Tx, typename _Tc>
class compound_type<_Tx _Tc::*>
{
public:
	static constexpr bool is_ptr = false;
	static constexpr bool is_ref = false;
	static constexpr bool is_array = false;
	static constexpr bool if_func = false;
	static constexpr bool is_ptr_mem = true;

	typedef _Tx _Tc::*base_type;
	typedef typename compound_type<_Tx>::bottom_type bottom_type;
	typedef _Tc class_type;
};

_GURU_END

#endif /* _GURU_TYPE_TRAITS_H_ */