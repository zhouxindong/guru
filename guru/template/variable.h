#pragma once
#include <array>

template <typename T>
constexpr T pi{ 3.1415926535897932385 };

template <typename T = long double>
constexpr T pi2 = T{ 3.141592635897932385 };

template <int N>
std::array<int, N> arr{};

// C++ 17
//template <auto N>
//constexpr decltype(N) dval{ N };

