#pragma once
#ifndef _GURU_MEM_TRACER_H_
#define _GURU_MEM_TRACER_H_

#include "../gvals.h"
#include <map>
#include <string>
#include <ostream>
#include <iomanip>
#include <algorithm>
#include <mutex>
#include <sstream>

#pragma warning(disable:4595 4291) 

_GURU_BEGIN

#pragma region alloc_rec

struct alloc_rec
{	// new alloc record item
	void* addr;
	std::string file;
	int line;
	size_t size;
};

inline 
std::ostream& 
operator << (std::ostream& out, alloc_rec const& r) noexcept
{
	out << "Addr:" << std::setw(16) << r.addr << " - Size: "
		<< r.size << " allocated in " << r.file << ":"
		<< r.line << "\n";
	return out;
}

#pragma endregion

#pragma region class alloc_tracer

class alloc_tracer
{
	friend std::ostream& operator << (std::ostream& out, alloc_tracer const& t) noexcept;

public:
	void add(alloc_rec const& r) noexcept
	{
		std::lock_guard<std::mutex> locker(_mutex);
		_rec[r.addr] = r;
	}

	void erase(void* ptr) noexcept
	{
		if (_rec.empty()) return;
		auto it = _rec.find(ptr);
		if (it == _rec.end()) return;
		_rec.erase(it);
	}

private:
	std::map<void*, alloc_rec> _rec;
	std::mutex _mutex;
};

static alloc_tracer _mem_tracer;	// instance used only

inline
std::ostream& 
operator << (std::ostream& out, alloc_tracer const& t) noexcept
{
	for_each(t._rec.cbegin(), t._rec.cend(),
		[&](std::pair<void*, alloc_rec> const& p)
	{
		out << p.second;
	});
	return out;
}

#pragma endregion

_GURU_END

#pragma region operators...

inline
void 
_record(void* ptr, size_t n, const char* name, int line) noexcept
{
	guru::alloc_rec r;
	r.addr = ptr;
	r.file = name;
	r.line = line;
	r.size = n;
	guru::_mem_tracer.add(r);
}

inline
void*
_trace_malloc(size_t n, const char* name, int line) noexcept
{
	void* ptr = malloc(n);
	_record(ptr, n, name, line);
	return ptr;
}

inline
void*
_trace_realloc(void* ptr, size_t n, const char* name, int line) noexcept
{
	guru::_mem_tracer.erase(ptr);
	void* ptr_ = realloc(ptr, n);
	_record(ptr_, n, name, line);
	return ptr_;
}

inline
void*
_trace_calloc(size_t count, size_t size, const char* name, int line) noexcept
{
	void* ptr = calloc(count, size);
	_record(ptr, count*size, name, line);
	return ptr;
}

inline
void
_trace_free(void* ptr) noexcept
{
	guru::_mem_tracer.erase(ptr);
	free(ptr);
}

inline
std::string
dump_mem_tracer() noexcept
{
	std::ostringstream oss;
	oss << guru::_mem_tracer;
	return oss.str();
}

inline
void* 
operator new(size_t n, const char* name, int line) 
{	
	return _trace_malloc(n, name, line);
}

inline
void*
operator new[](size_t n, const char* name, int line) 
{	
	return _trace_malloc(n, name, line);
}

inline
void 
operator delete(void* ptr) noexcept
{	
	_trace_free(ptr);
}

inline
void
operator delete[](void* ptr) noexcept
{
	_trace_free(ptr);
}

inline
void 
operator delete(void* ptr, const char* name, int line) noexcept
{
	_trace_free(ptr);
}

inline
void
operator delete[](void* ptr, const char* name, int line) noexcept
{
	_trace_free(ptr);
}

#define tracingnew new (__FILE__, __LINE__)
#define new tracingnew
#define tracingDelete delete
#define delete tracingDelete

#define malloc(s) _trace_malloc(s, __FILE__, __LINE__)
#define realloc(p,s) _trace_realloc(p, s, __FILE__, __LINE__)
#define calloc(n,s) _trace_calloc(n, s, __FILE__, __LINE__)
#define free(p) _trace_free(p)

#pragma endregion

#endif /* _GURU_MEM_TRACER_H_ */

