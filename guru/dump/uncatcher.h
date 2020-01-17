#pragma once
#ifndef _GURU_UNCATCHER_H_
#define _GURU_UNCATCHER_H_

#include "../gvals.h"
#include "stack_tracer.h"
#include "struct_exception.h"
#include <functional>

_GURU_BEGIN

enum class Unhandle_Result
{
	Quit		= EXCEPTION_EXECUTE_HANDLER,
	OS_Def		= EXCEPTION_CONTINUE_SEARCH,
	Continue	= EXCEPTION_CONTINUE_EXECUTION
};

bool DisableSetUnhandledExceptionFilter();

/*
** unhandled exception call back function
** caution: only the last uncatcher object ctor set it on use
** before cb would be covered
*/
static std::function<void(std::string)> uncatched_call_back;

/*
** 1. catch all unhandled exception
** 2. translate the SH to struct_exception object
*/
class uncatcher
{
	friend LONG WINAPI _UnhandledException(_EXCEPTION_POINTERS *);
	friend void _se_translator(unsigned u, EXCEPTION_POINTERS*);

public:
	uncatcher(std::function<void(std::string)> cb) noexcept
	{
		uncatched_call_back = cb;
		SetUnhandledExceptionFilter(_UnhandledException);
		DisableSetUnhandledExceptionFilter();
		_old_se_translator = _set_se_translator(_se_translator);
	}

	~uncatcher() noexcept
	{
		_set_se_translator(_old_se_translator);
	}

private:
	_se_translator_function _old_se_translator;
};

inline
LONG 
WINAPI
_UnhandledException(_EXCEPTION_POINTERS *)
{
	auto s = stack_tracer::stack_trace(
		"****** [Unhandled Exception] ******",
		"****** [End Unhandled] ******");
	uncatched_call_back(s);

	FatalAppExitA(0, "Unhandled Exception! App would abort!");
	return (LONG)Unhandle_Result::OS_Def;
}

inline
void 
_se_translator(unsigned u, EXCEPTION_POINTERS *ep)
{
	throw struct_exception(struct_exception::se_info(ep, true, u));
}

// after success call it, SetUnhandledExceptionFilter() call would be invalid
bool inline
DisableSetUnhandledExceptionFilter()
{
	void* addr = (void*)GetProcAddress(LoadLibraryA("kernel32.dll"),
		"SetUnhandledExceptionFilter");

	if (addr)
	{
		unsigned char code[16];
		int size = 0;

		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;

		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
		return true;
	}
	return false;
}

_GURU_END

#endif /* _GURU_UNCATCHER_H_ */