#pragma once
#ifndef _GURU_STRUCT_EXCEPTION_H_
#define _GURU_STRUCT_EXCEPTION_H_

#include "../gvals.h"
#include <windows.h>
#include <eh.h>
#include <Psapi.h>
#include <sstream>
#include <exception>
#include <string>
#include "stack_tracer.h"

#pragma warning(disable:4275 4251) // disable warning C4275, C4251

// Compile with /EHa (C/C++ -> Code Generation -> Enable C++ Exceptions)

_GURU_BEGIN

/*
** SEH
** need /EHa compile option
*/
class struct_exception : public std::exception
{
public:
	struct_exception(
		const std::string message = "unknown exception",
		const std::string type = "struct_exception") noexcept
		: _type(type), _message(message)
	{
		_calc_stack();
		_what = std::string("\n\t***** " + _type + " *****\n\t[info]: " +
			_message + '\n' + _stack);
	}

	virtual 
		~struct_exception() noexcept
	{
	}

	const char*
		what() const noexcept override
	{
		return _what.c_str();
	}

private:
	std::string _type;
	std::string _stack;
	std::string _message;
	std::string _what;

	void 
		_calc_stack() noexcept
	{
		_stack = guru::stack_tracer::stack_trace("[Begin Call Stack]", "[End Call Stack]");
	}

private:
	static 
	const char* 
		_op_desc(const ULONG op_code) noexcept
	{
		switch (op_code)
		{
		case 0:
			return "read";
		case 1: 
			return "write";
		case 8:
			return "user-mode data execution prevention (DEP) violation";
		default: 
			return "unknown";
		}
	}

	static
	const char* 
		_se_desc(const unsigned int& code) noexcept
	{
		switch (code)
		{
		case EXCEPTION_ACCESS_VIOLATION:         
			return "EXCEPTION_ACCESS_VIOLATION";
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    
			return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		case EXCEPTION_BREAKPOINT:               
			return "EXCEPTION_BREAKPOINT";
		case EXCEPTION_DATATYPE_MISALIGNMENT:    
			return "EXCEPTION_DATATYPE_MISALIGNMENT";
		case EXCEPTION_FLT_DENORMAL_OPERAND:     
			return "EXCEPTION_FLT_DENORMAL_OPERAND";
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:       
			return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		case EXCEPTION_FLT_INEXACT_RESULT:       
			return "EXCEPTION_FLT_INEXACT_RESULT";
		case EXCEPTION_FLT_INVALID_OPERATION:    
			return "EXCEPTION_FLT_INVALID_OPERATION";
		case EXCEPTION_FLT_OVERFLOW:             
			return "EXCEPTION_FLT_OVERFLOW";
		case EXCEPTION_FLT_STACK_CHECK:          
			return "EXCEPTION_FLT_STACK_CHECK";
		case EXCEPTION_FLT_UNDERFLOW:            
			return "EXCEPTION_FLT_UNDERFLOW";
		case EXCEPTION_ILLEGAL_INSTRUCTION:      
			return "EXCEPTION_ILLEGAL_INSTRUCTION";
		case EXCEPTION_IN_PAGE_ERROR:            
			return "EXCEPTION_IN_PAGE_ERROR";
		case EXCEPTION_INT_DIVIDE_BY_ZERO:       
			return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		case EXCEPTION_INT_OVERFLOW:             
			return "EXCEPTION_INT_OVERFLOW";
		case EXCEPTION_INVALID_DISPOSITION:      
			return "EXCEPTION_INVALID_DISPOSITION";
		case EXCEPTION_NONCONTINUABLE_EXCEPTION: 
			return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		case EXCEPTION_PRIV_INSTRUCTION:         
			return "EXCEPTION_PRIV_INSTRUCTION";
		case EXCEPTION_SINGLE_STEP:              
			return "EXCEPTION_SINGLE_STEP";
		case EXCEPTION_STACK_OVERFLOW:           
			return "EXCEPTION_STACK_OVERFLOW";
		default: 
			return "UNKNOWN EXCEPTION";
		}
	}

public:
	static 
	const char* 
		se_info(
			struct _EXCEPTION_POINTERS* ep,
			bool has_exception_code = false, unsigned int code = 0) noexcept
	{
		HMODULE hm;
		::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, static_cast<LPCTSTR>(ep->ExceptionRecord->ExceptionAddress), &hm);
		MODULEINFO mi;
		::GetModuleInformation(::GetCurrentProcess(), hm, &mi, sizeof(mi));
		char fn[MAX_PATH];
		::GetModuleFileNameExA(::GetCurrentProcess(), hm, fn, MAX_PATH);

		std::ostringstream oss;
		oss << "SE " << (has_exception_code ? _se_desc(code) : "") 
			<< " at address 0x" << std::hex << ep->ExceptionRecord->ExceptionAddress << std::dec
			<< " inside " << fn << " loaded at base address 0x" << std::hex << mi.lpBaseOfDll << "\n";

		if (has_exception_code && (
			code == EXCEPTION_ACCESS_VIOLATION ||
			code == EXCEPTION_IN_PAGE_ERROR)) 
		{
			oss << "Invalid operation: " 
				<< _op_desc((ULONG)ep->ExceptionRecord->ExceptionInformation[0]) 
				<< " at address 0x" << std::hex << ep->ExceptionRecord->ExceptionInformation[1] 
				<< std::dec << "\n";
		}

		if (has_exception_code && code == EXCEPTION_IN_PAGE_ERROR) 
		{
			oss << "Underlying NTSTATUS code that resulted in the exception " 
				<< ep->ExceptionRecord->ExceptionInformation[2] << "\n";
		}

		return oss.str().c_str();
	}
};

_GURU_END

#endif /* _GURU_STRUCT_EXCEPTION_H_ */