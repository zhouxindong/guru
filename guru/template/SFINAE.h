#pragma once

#include <utility>
#include <type_traits>

// SFINAE principle turns potential errors during the formation of invalid types and expressions
// during template argument deduction into simple deduction failures, allowing overload
// resolution to select a different candidate
// SFINAE also enables compile-time techniques that can determine if a particular type or expression
// is valid

// 1. SFINAE out functions overload

template <typename T1, typename T2>
class HasLess
{
	template <typename T>
	struct Identity;

	template <typename U1, typename U2>
	static std::true_type
		test(Identity<decltype(std::declval<U1>() < std::declval<U2>())*);

	template <typename U1, typename U2>
	static std::false_type
		test(...);

public:
	static constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

// 2. to SFINAE out partial specialization
template <typename...>
using VoidT = void;

template <typename, typename = VoidT<>>
struct IsDefaultConstructibleT : std::false_type
{
};

template <typename T>
struct IsDefaultConstructibleT<T, VoidT<decltype(T())>> : std::true_type
{
};