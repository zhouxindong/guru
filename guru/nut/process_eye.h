#pragma once

#ifndef _GURU_PROCESS_EYE_H_
#define _GURU_PROCESS_EYE_H_

#include "../gvals.h"
#include "event_base.h"
#include "eye_base.h"

#include <list>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <windows.h>
#include <Psapi.h>

_GURU_BEGIN

class process_event : public event_base
{
public:
	process_event(DWORD const& pid, std::string& name) noexcept :
		_proc_id{ pid },
		_proc_name{ name }
	{
		_type = EVENT_TYPE::EVENT_PROCESS;
		::GetLocalTime(&_start_time);
	}

	process_event(process_event const& other) noexcept 
		: event_base::event_base{other},
		_proc_id {other._proc_id},
		_proc_name{other._proc_name},
		_start_time{other._start_time},
		_end_time{other._end_time}
	{
	}

	~process_event()
	{
	}

public:

	SYSTEMTIME const& start_time() const noexcept
	{
		return _start_time;
	}

	SYSTEMTIME const& end_time() const noexcept
	{
		return _end_time;
	}

	DWORD const& proc_id() const noexcept
	{
		return _proc_id;
	}

	std::string const& proc_name() const noexcept
	{
		return _proc_name;
	}

	void end_proc() noexcept
	{
		::GetLocalTime(&_end_time);
	}

public:
	std::string to_string() const noexcept
	{
		std::ostringstream stream;
		stream << event_base::to_string();
		stream << "[Process Event] ";
		stream << "Start time: " << _start_time.wHour << ":" << _start_time.wMinute 
			<< ":" << _start_time.wSecond << ":" << _start_time.wMilliseconds << ", ";
		stream << "End time: " << _end_time.wHour << ":" << _end_time.wMinute 
			<< ":" << _end_time.wSecond << ":" << _end_time.wMilliseconds << ", ";
		stream << "ProcessId: " << _proc_id << ", ";
		stream << "ProcessName: " << _proc_name;
		return stream.str();
	}

private:
	DWORD _proc_id;
	std::string _proc_name;

	SYSTEMTIME _start_time;
	SYSTEMTIME _end_time;
};

class process_eye : public eye_base
{
public:
	void operator()() noexcept override
	{
		int count = 0;
		DWORD processIds[256];
		DWORD pBytesReturned;
		DWORD processIdsSize = sizeof(processIds);

		while (true)
		{
			if (!EnumProcesses(processIds, processIdsSize, &pBytesReturned))
			{
				continue;
			}

			DWORD numProcesses = pBytesReturned / sizeof(DWORD);

			// Find processes that have already finsihed
			auto iter = _event_list.begin();
			while (iter != _event_list.end())
			{
				std::shared_ptr<process_event> processEvent = *iter;

				bool foundProcess = false;
				for (DWORD i = 0; i < numProcesses; ++i)
				{
					if (processIds[i] == processEvent->proc_id())
					{
						foundProcess = true;
						break;
					}
				}

				if (!foundProcess)
				{
					std::ostringstream oss;
					oss << "Finished PID: " << processEvent->proc_id() << " : " << processEvent->proc_name() << std::endl;
					_signal(oss.str());

					// Remove finished process
					processEvent->end_proc();
					_id_map.erase(_id_map.find(processEvent->proc_id()));
					_finished_list.emplace_back(processEvent);
					iter = _event_list.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			// Find new process and add to list and map
			for (DWORD i = 0; i < numProcesses; ++i)
			{
				if (_id_map.find(processIds[i]) == _id_map.end())
				{
					std::ostringstream oss;
					oss << "Add new PID: " << processIds[i] << std::endl;
					_signal(oss.str());
					_new_proc(processIds[i]);
				}
			}
		}
	}

private:
	void _new_proc(DWORD id) noexcept
	{
		HANDLE process = OpenProcess(
			PROCESS_ALL_ACCESS,
			FALSE,
			id);

		std::string processName = "<unknown>";

		if (process != nullptr)
		{
			HMODULE moduleHandles[256];
			DWORD numBytesReturned;

			if (EnumProcessModules(
				process,
				moduleHandles,
				sizeof(moduleHandles),
				&numBytesReturned))
			{
				DWORD numModules = numBytesReturned / sizeof(HMODULE);
				{
					std::ostringstream oss;
					oss << "Found " << numModules << " modules" << std::endl;
					_signal(oss.str());
				}

				char processNameChar[MAX_PATH] = "<unknown>";

				GetModuleBaseNameA(
					process,
					moduleHandles[0],
					processNameChar,
					sizeof(processNameChar) / sizeof(TCHAR));

				{
					std::ostringstream oss;
					oss << "ProcessName: " << processNameChar << " (PID: " << id << ")" << std::endl;
					_signal(oss.str());
				}
				
				processName = std::string(processNameChar);
			}
		}
		else
		{
			std::ostringstream oss;
			oss << "Unexpected can't find PID " << id << std::endl;
			_signal(oss.str());
		}

		process_event* event = new process_event(id, processName);

		std::shared_ptr<process_event> eventPtr;
		eventPtr.reset(event);

		_id_map.emplace(id, eventPtr);
		_event_list.emplace_back(eventPtr);

		CloseHandle(process);
	}

private:
	std::unordered_map<DWORD, std::shared_ptr<process_event>> _id_map;
	std::list<std::shared_ptr<process_event>> _event_list;
	std::list<std::shared_ptr<process_event>> _finished_list;
};

_GURU_END

#endif /* _GURU_PROCESS_EYE_H_ */