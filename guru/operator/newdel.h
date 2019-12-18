#pragma once
#include <string>

// overload global new operator (not replace it, more safe)
void* operator new(size_t n, const std::string& pat)
{
	char* p = static_cast<char*>(::operator new(n));
	const char* pattern = pat.c_str();
	if (!pattern || !pattern[0])
		pattern = "\0";	// note: two null chars
	const char* f = pattern;
	for (int i = 0; i < n; ++i)
	{
		if (!*f)
			f = pattern;
		p[i] = *f++;
	}
	return p;
}

void operator delete(void* p, const std::string& pat)
{
	::operator delete(p);
}