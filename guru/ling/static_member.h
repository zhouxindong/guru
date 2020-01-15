#pragma once

struct BigInt
{
	BigInt(long long) {}
};

class Foo
{
public:
	static int const zero = 0;
	static constexpr float dzero = 0.0f;
	//inline static BigInt const bigzero = BigInt{ 0 }; // C++17

	static constexpr double ddzero()
	{
		return 0.0;
	}

	static BigInt bigzero()
	{
		return BigInt{ 0 };
	}
};