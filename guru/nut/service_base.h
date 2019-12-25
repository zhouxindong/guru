#pragma once
#ifndef _GURU_SERVICE_BASE_H_
#define _GURU_SERVICE_BASE_H_

#include "../gvals.h"
#include <windows.h>
#include <cassert>
#include <atlstr.h>

_GURU_BEGIN

class service_base;

// start service, register service main
bool _run_internal(service_base* svc) noexcept;

/*
** windows service base class
*/
class service_base
{
	// service main, start service
	friend void WINAPI _service_main(DWORD, TCHAR*[]) noexcept;
	// used by service main, register scm operate
	friend DWORD WINAPI	_service_ctrl_handler(DWORD, DWORD, void*, void*) noexcept;

public:
	service_base(const service_base& other) = delete;
	service_base& operator=(const service_base& other) = delete;

	service_base(service_base&& other) = delete;
	service_base& operator=(service_base&& other) = delete;

	virtual ~service_base() {}

public:

	bool run() noexcept
	{	// Called by windows when starting the service.
		return _run_internal(this);
	}

	const CString&	name() const noexcept			{ return _name; }
	const CString&	disp_name() const noexcept		{ return _disp_name; }
	const DWORD		start_type() const noexcept		{ return _start_type; }
	const DWORD		error_ctrl_type() const noexcept{ return _error_ctrl_type; }

	const CString&	depends() const noexcept		{ return _depends; }
	const CString&	account() const noexcept		{ return _account; }
	const CString&	password() const noexcept		{ return _password; }

protected:
	service_base(
		const CString& name,
		const CString& disp_name,
		DWORD start_type,
		DWORD err_ctrl_type = SERVICE_ERROR_NORMAL,
		DWORD _accepted_cmds = SERVICE_ACCEPT_STOP,
		const CString& depends = _T(""),
		const CString& account = _T(""),
		const CString& password = _T("")) noexcept
		: _name(name),
		_disp_name(disp_name),
		_start_type(start_type),
		_error_ctrl_type(err_ctrl_type),
		_depends(depends),
		_account(account),
		_password(password)
	{
		_svc_status.dwControlsAccepted = _accepted_cmds;
		_svc_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		_svc_status.dwCurrentState = SERVICE_START_PENDING;
		_svc_status.dwWin32ExitCode = NO_ERROR;
		_svc_status.dwServiceSpecificExitCode = 0;
		_svc_status.dwCheckPoint = 0;
		_svc_status.dwWaitHint = 0;
	}

	void set_status(DWORD state, DWORD err_code = NO_ERROR, DWORD wait = 0) noexcept
	{
		_svc_status.dwCurrentState = state;
		_svc_status.dwWin32ExitCode = err_code;
		_svc_status.dwWaitHint = wait;
		::SetServiceStatus(_svc_status_handle, &_svc_status);
	}

	// Override these functions as you need.
	virtual void on_start(DWORD argc, TCHAR* argv[]) = 0;
	virtual void on_stop() {}
	virtual void on_pause() {}
	virtual void on_continue() {}
	virtual void on_shutdown() {}

	virtual void on_session_change(DWORD /*evt_type*/,
		WTSSESSION_NOTIFICATION* /*notification*/) {}

private:
	void start(DWORD argc, TCHAR* argv[])
	{
		set_status(SERVICE_START_PENDING);
		on_start(argc, argv);
		set_status(SERVICE_RUNNING);
	}

	void stop()
	{
		set_status(SERVICE_STOP_PENDING);
		on_stop();
		set_status(SERVICE_STOPPED);
	}

	void pause()
	{
		set_status(SERVICE_PAUSE_PENDING);
		on_pause();
		set_status(SERVICE_PAUSED);
	}

	void continued()
	{
		set_status(SERVICE_CONTINUE_PENDING);
		on_continue();
		set_status(SERVICE_RUNNING);
	}

	void shutdown()
	{
		on_shutdown();
		set_status(SERVICE_STOPPED);
	}

private:
	CString _name;
	CString _disp_name;
	DWORD _start_type;
	DWORD _error_ctrl_type;

private: 
	CString _depends;
	CString _account;
	CString _password;

private:
	SERVICE_STATUS _svc_status;
	SERVICE_STATUS_HANDLE _svc_status_handle{ nullptr };
};

static service_base* _service;

inline
DWORD
WINAPI
_service_ctrl_handler(
	DWORD ctrl_code, 
	DWORD evt_type, 
	void* evt_data, 
	void* context) noexcept
{	// Called whenever service control manager updates service status.
	switch (ctrl_code) {
	case SERVICE_CONTROL_STOP:
		_service->stop();
		break;

	case SERVICE_CONTROL_PAUSE:
		_service->pause();
		break;

	case SERVICE_CONTROL_CONTINUE:
		_service->continued();
		break;

	case SERVICE_CONTROL_SHUTDOWN:
		_service->shutdown();
		break;

	case SERVICE_CONTROL_SESSIONCHANGE:
		_service->on_session_change(
			evt_type, reinterpret_cast<WTSSESSION_NOTIFICATION*>(evt_data));
		break;

	default:
		break;
	}

	return 0;
}

inline
void 
write_event_log(
	CString const& name, 
	CString const& msg, 
	WORD type = EVENTLOG_INFORMATION_TYPE) noexcept
{
	HANDLE h = RegisterEventSource(nullptr, name);
	if (h) 
	{
		const TCHAR* d[2] = { name, msg };
		ReportEvent(h, type, 0, 0, nullptr, 2, 0, d, nullptr);
		DeregisterEventSource(h);
	}
}

inline
void 
WINAPI 
_service_main(DWORD argc, TCHAR* argv[]) noexcept
{	// registers handle and starts the service
	assert(_service);
	_service->_svc_status_handle = 
		::RegisterServiceCtrlHandlerEx(_service->name(), _service_ctrl_handler, NULL);
	if (!_service->_svc_status_handle) 
	{
		write_event_log(
			_service->name(), _T("Can't set service control handler"), EVENTLOG_ERROR_TYPE);
		return;
	}
	_service->start(argc, argv);
}

inline
bool 
_run_internal(service_base* svc) noexcept
{
	_service = svc;

	TCHAR* svc_name = const_cast<CString&>(_service->name()).GetBuffer();

	SERVICE_TABLE_ENTRY tableEntry[] = {
		{ svc_name, _service_main },
		{ nullptr, nullptr }
	};

	return ::StartServiceCtrlDispatcher(tableEntry) == TRUE;
}

_GURU_END

#endif /* _GURU_SERVICE_BASE_H_ */