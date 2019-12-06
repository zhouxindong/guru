#pragma once

#ifndef _GURU_STACK_TRACER_H_
#define _GURU_STACK_TRACER_H_

#include "../gvals.h"
#include <windows.h>
#include <Psapi.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

#pragma pack( push, before_imagehlp, 8 )
#include <imagehlp.h>
#pragma pack( pop, before_imagehlp )

_GURU_BEGIN

class stack_tracer
{
	struct module_data
	{
		std::string image_name;
		std::string module_name;
		void *base_address;
		DWORD load_size;
	};

	class symbol_handler 
	{
		HANDLE p;
	public:
		symbol_handler(HANDLE process, char const *path = NULL, bool intrude = false) :
			p(process)
		{
			if (!SymInitialize(p, path, intrude))
				throw(std::logic_error("Unable to initialize symbol handler"));
		}
		~symbol_handler() noexcept
		{ 
			SymCleanup(p); 
		}
	};

	class get_mod_info 
	{
		HANDLE process;
		static const int buffer_length = 4096;
	public:
		get_mod_info(HANDLE h) noexcept : process(h) 
		{
		}

		module_data 
		operator () (HMODULE module) noexcept
		{
			module_data ret;
			char temp[buffer_length];
			MODULEINFO mi;

			GetModuleInformation(process, module, &mi, sizeof(mi));
			ret.base_address = mi.lpBaseOfDll;
			ret.load_size = mi.SizeOfImage;

			GetModuleFileNameExA(process, module, temp, sizeof(temp));
			ret.image_name = temp;
			GetModuleBaseNameA(process, module, temp, sizeof(temp));
			ret.module_name = temp;
			std::vector<char> img(ret.image_name.begin(), ret.image_name.end());
			std::vector<char> mod(ret.module_name.begin(), ret.module_name.end());
			SymLoadModule64(process, 0, &img[0], &mod[0], (DWORD64)ret.base_address, ret.load_size);
			return ret;
		}
	};

	class symbol 
	{
		typedef IMAGEHLP_SYMBOL64 sym_type;
		sym_type *sym;
		static const int max_name_len = 1024;

	public:
		symbol(HANDLE process, DWORD64 address) noexcept : 
			sym((sym_type *)::operator new(sizeof(*sym) + max_name_len)) 
		{
			memset(sym, '\0', sizeof(*sym) + max_name_len);
			sym->SizeOfStruct = sizeof(*sym);
			sym->MaxNameLength = max_name_len;
			DWORD64 displacement;

			if (!SymGetSymFromAddr64(process, address, &displacement, sym))
				throw(std::logic_error("Bad symbol"));
		}

		std::string 
		name() noexcept
		{ 
			return std::string(sym->Name); 
		}

		std::string 
		undecorated_name() noexcept
		{
			std::vector<char> und_name(max_name_len);
			UnDecorateSymbolName(sym->Name, &und_name[0], max_name_len, UNDNAME_COMPLETE);
			return std::string(&und_name[0], strlen(&und_name[0]));
		}

		static 
		void 
			options(DWORD add, DWORD remove = 0) noexcept
		{
			DWORD symOptions = SymGetOptions();
			symOptions |= add;
			symOptions &= ~remove;
			SymSetOptions(symOptions);
		}
	};

public:
	static
	std::string 
		stack_trace(const std::string& prefix = "", const std::string& surfix = "") noexcept
	{
		std::ostringstream os;

		HANDLE process = GetCurrentProcess();
		HANDLE hThread = GetCurrentThread();

		CONTEXT c;
		memset(&c, 0, sizeof(CONTEXT));

#ifdef _M_IX86
		c.ContextFlags = CONTEXT_CONTROL;

		__asm
		{
		Label:
			mov[c.Ebp], ebp;
			mov[c.Esp], esp;
			mov eax, [Label];
			mov[c.Eip], eax;
		}
#else
		c.ContextFlags = CONTEXT_FULL;
		RtlCaptureContext(&c);
#endif

		int frame_number = 0;
		DWORD offset_from_symbol = 0;
		IMAGEHLP_LINE64 line = { 0 };

		symbol_handler handler(process);
		symbol::options(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);

		void *base = _load_modules_symbols(process, GetCurrentProcessId());

		STACKFRAME64 s = _init_stack_frame(c);

		line.SizeOfStruct = sizeof line;

		IMAGE_NT_HEADERS *h = ImageNtHeader(base);
		DWORD image_type = h->FileHeader.Machine;

		os << "\n" << "\t\t" << prefix << "\n";
		do 
		{
			if (!StackWalk64(image_type, process, hThread, &s, &c, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
				return std::string();

			os << std::setw(3) << "\n" << frame_number << "\t";
			if (s.AddrPC.Offset != 0) 
			{
				os << symbol(process, s.AddrPC.Offset).undecorated_name();

				if (SymGetLineFromAddr64(process, s.AddrPC.Offset, &offset_from_symbol, &line))
					os << "\t" << line.FileName << "(" << line.LineNumber << ")";
			}
			else
				os << "(No Symbols: PC == 0)";

			++frame_number;
		} while (s.AddrReturn.Offset != 0);

		os << "\n\n" << "\t\t" << surfix << "\n";

		return os.str();
	}

private:
	static 
	void*
		_load_modules_symbols(HANDLE process, DWORD pid) noexcept
	{
		std::vector<module_data> modules;
		DWORD cbNeeded;
		std::vector<HMODULE> module_handles(1);
		EnumProcessModules(process, &module_handles[0], DWORD(module_handles.size() * sizeof(HMODULE)), &cbNeeded);
		module_handles.resize(cbNeeded / sizeof(HMODULE));
		EnumProcessModules(process, &module_handles[0], DWORD(module_handles.size() * sizeof(HMODULE)), &cbNeeded);

		std::transform(module_handles.begin(), module_handles.end(), std::back_inserter(modules), get_mod_info(process));
		return modules[0].base_address;
	}

	static 
	STACKFRAME64 
		_init_stack_frame(CONTEXT c) noexcept
	{
#ifdef _M_X64
		STACKFRAME64 s;
		s.AddrPC.Offset = c.Rip;
		s.AddrPC.Mode = AddrModeFlat;
		s.AddrStack.Offset = c.Rsp;
		s.AddrStack.Mode = AddrModeFlat;
		s.AddrFrame.Offset = c.Rbp;
		s.AddrFrame.Mode = AddrModeFlat;
		return s;
#else
		STACKFRAME64 s;
		s.AddrPC.Offset = c.Eip;
		s.AddrPC.Mode = AddrModeFlat;
		s.AddrStack.Offset = c.Esp;
		s.AddrStack.Mode = AddrModeFlat;
		s.AddrFrame.Offset = c.Ebp;
		s.AddrFrame.Mode = AddrModeFlat;
		return s;
#endif
	}
};

_GURU_END

#endif /* _GURU_STACK_TRACER_H_ */