#pragma once
#ifndef _GURU_PROC_INFO_H_
#define _GURU_PROC_INFO_H_

#include "../gvals.h"
#include <ratio>
#include <windows.h>
#include <psapi.h>
#include <type_traits>
#include <string>
#include <sstream>
#include <iomanip>

#pragma comment(lib,"psapi.lib")

_GURU_BEGIN

inline
void 
gc() noexcept
{	// garbage collect for current process
	EmptyWorkingSet(GetCurrentProcess());
}

enum class PROC_MEMORY_TYPE
{
	WORKING,		// WorkingSetSize
	PEAK_WORKING,	// PeakWorkingSetSize
	PAGE,			// PagefileUsage
	PEAK_PAGE,		// PeakPagefileUsage
	TOTAL,			// WorkingSetSize + PagefileUsage
	TOTAL_PEAK,		// PeakWorkingSetSize + PeakPagefileUsage
};

template <typename _Tx>
std::string 
to_quantity(_Tx const& v, const char* q) noexcept
{
	std::ostringstream oss;
	oss << std::showpoint << std::fixed << std::setprecision(1)
		<< v << "[" << q << "]";
	return oss.str();
}

template <typename _Quan, typename _Tx>
struct quantity_format
{
	static std::string to_string(_Tx const& v) noexcept
	{
		return to_quantity(v, "??");
	}
};

template <typename _Tx>
struct quantity_format<std::mega, _Tx>
{
	static std::string to_string(_Tx const& v) noexcept
	{
		return to_quantity(v, "MB");
	}
};

template <typename _Tx>
struct quantity_format<std::giga, _Tx>
{
	static std::string to_string(_Tx const& v) noexcept
	{
		return to_quantity(v, "GB");
	}
};

template <typename _Tx>
struct quantity_format<std::kilo, _Tx>
{
	static std::string to_string(_Tx const& v) noexcept
	{
		return to_quantity(v, "KB");
	}
};

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

	decay_Tx working_size() noexcept
	{
		return _calc(_pmc.WorkingSetSize);
	}

	std::string working_size_string() noexcept
	{
		return quantity_format<_Quan, _Tx>::to_string(working_size());
	}

	decay_Tx peak_working_size() noexcept
	{
		return _calc(_pmc.PeakWorkingSetSize);
	}

	std::string peak_working_size_string() noexcept
	{
		return quantity_format<_Quan, _Tx>::to_string(peak_working_size());
	}

	decay_Tx page_file() noexcept
	{
		return _calc(_pmc.PagefileUsage);
	}

	std::string page_file_string() noexcept
	{
		return quantity_format<_Quan, _Tx>::to_string(page_file());
	}

	decay_Tx peak_page_file() noexcept
	{
		return _calc(_pmc.PeakPagefileUsage);
	}

	std::string peak_page_file_string() noexcept
	{
		return quantity_format<_Quan, _Tx>::to_string(peak_page_file());
	}

	decay_Tx total() noexcept
	{
		return working_size() + page_file();
	}

	std::string total_string() noexcept
	{
		return quantity_format<_Quan, _Tx>::to_string(total());
	}

	decay_Tx total_peak() noexcept
	{
		return peak_working_size() + peak_page_file();
	}

	std::string total_peak_string() noexcept
	{
		return quantity_format<_Quan, _Tx>::to_string(total_peak());
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

inline
LONGLONG 
file_time_2_utc(const FILETIME* ftime) noexcept
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

inline
DWORD 
get_processor_number() noexcept
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

int get_cpu_usage(int pid)
{
	static int processor_count_ = -1;

	static __int64 last_time_ = 0;
	static __int64 last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	__int64 system_time;
	__int64 time;
	__int64 system_time_delta;
	__int64 time_delta;

	int cpu = -1;

	if (processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	if (time_delta == 0)
		return -1;

	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}
_GURU_END

#endif /* _GURU_PROC_INFO_H_ */