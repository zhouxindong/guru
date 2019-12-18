#pragma once
#ifndef _GURU_PROC_INFO_H_
#define _GURU_PROC_INFO_H_

#include "../gvals.h"
#include <ratio>
#include <windows.h>
#include <psapi.h>
#include <type_traits>

#pragma comment(lib,"psapi.lib")

_GURU_BEGIN



template <typename _Quan = std::mega, typename _Tx = float>
struct proc_mem
{
	typedef proc_mem<_Quan, _Tx> type;
	typedef _Quan quan_type;
	typedef _Tx value_type;

private:
	typedef std::decay_t<_Tx> decay_Tx;

public:
	proc_mem() noexcept
	{
		static_assert(std::is_arithmetic_v<_Tx>, "_Tx must be arithemic type");
	}

	void fresh() noexcept
	{	// fresh memory info
		GetProcessMemoryInfo(GetCurrentProcess(), &_pmc, (DWORD)_pmc_size);
	}

	void gc() noexcept
	{
		EmptyWorkingSet(GetCurrentProcess());
	}

	decay_Tx working_size() noexcept
	{
		return _calc(_pmc.WorkingSetSize);
	}

	decay_Tx peak_working_size() noexcept
	{
		return _calc(_pmc.PeakWorkingSetSize);
	}

	decay_Tx page_file() noexcept
	{
		return _calc(_pmc.PagefileUsage);
	}

	decay_Tx peak_page_file() noexcept
	{
		return _calc(_pmc.PeakPagefileUsage);
	}

private:
	decay_Tx _calc(size_t size) noexcept
	{
		return ((decay_Tx)size) / _quan;
	}

private:
	PROCESS_MEMORY_COUNTERS _pmc;

private:
	size_t _pmc_size{ sizeof(PROCESS_MEMORY_COUNTERS) };
	decay_Tx _quan{ (decay_Tx)quan_type::num / (decay_Tx)quan_type::den };
};

_GURU_END

#endif /* _GURU_PROC_INFO_H_ */