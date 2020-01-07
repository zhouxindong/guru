#pragma once

auto l = [](int i) {
	return i < 0;
}

// a instance of closure type, which has a function call operator
// captured variables become data members

/**
 * class SomeCompilerSpecificNameX
 * {
 *	public:
 *		SomeCompilerSpecificNameX();
 *		bool operator() (int i) const
 *		{
 *			return i < 0;
 *		}
 * };
 */