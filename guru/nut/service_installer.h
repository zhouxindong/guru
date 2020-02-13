#pragma once
#ifndef _GURU_SERVICE_INSTALLER_H_
#define _GURU_SERVICE_INSTALLER_H_

#include "../gvals.h"
#include "service_base.h"

_GURU_BEGIN

class service_handle
{
public:
	service_handle(SC_HANDLE h) noexcept
		:_handle{ h }
	{
	}

	~service_handle() noexcept
	{
		if (_handle != nullptr)
			::CloseServiceHandle(_handle);
	}

public:
	operator SC_HANDLE() noexcept
	{
		return _handle;
	}

private:
	SC_HANDLE _handle{ nullptr };
};

inline
bool
create_service(service_base const& svc)
{
	CString esc_path;
	TCHAR* m_path = esc_path.GetBufferSetLength(MAX_PATH);

	if (::GetModuleFileName(nullptr, m_path, MAX_PATH) == 0) 
	{
		_tprintf(_T("Couldn't get module file name: %d\n"), ::GetLastError());
		return false;
	}

	esc_path.ReleaseBuffer();
	esc_path.Remove(_T('\"'));

	esc_path = _T('\"') + esc_path + _T('\"');

	service_handle scm = ::OpenSCManager(nullptr, nullptr,
		SC_MANAGER_CONNECT |
		SC_MANAGER_CREATE_SERVICE);
	if (!scm) 
	{
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	const CString& depends = svc.depends();
	const CString& acc = svc.account();
	const CString& pass = svc.password();

	service_handle h = ::CreateService(
		scm,
		svc.name(),
		svc.disp_name(),
		SERVICE_QUERY_STATUS,
		SERVICE_WIN32_OWN_PROCESS,
		svc.start_type(),
		svc.error_ctrl_type(),
		esc_path,
		nullptr,
		nullptr,
		(depends.IsEmpty() ? nullptr : depends.GetString()),
		(acc.IsEmpty() ? nullptr : acc.GetString()),
		(pass.IsEmpty() ? nullptr : pass.GetString()));
	if (!h) 
	{
		_tprintf(_T("Couldn't create service: %d\n"), ::GetLastError());
		return false;
	}

	return true;
}

inline 
void
install_service(service_base const& svc)
{
	_tprintf(_T("Installing service\n"));
	CString s;
	s.AppendFormat(_T("Installing %s service\n"), svc.name());
	OutputDebugString(s);
	if (!create_service(svc)) {
		_tprintf(_T("create service failed\n"));
		s.Empty();
		s.AppendFormat(_T("Couldn't create %s service: %d\n"), svc.name(), ::GetLastError());
		OutputDebugString(s);
		return;
	}

	_tprintf(_T("Create service successful\n"));
	s.Empty();
	s.AppendFormat(_T("Service %s created\n"), svc.name());
	OutputDebugString(s);

	if ((svc.start_type() != SERVICE_AUTO_START) || (svc.delay_start() != true))
		return;

	service_handle scm = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if (!scm)
	{
		_tprintf(_T("OpenSCManager failed, please manual to Auto Delay!\n"));
		s.Empty();
		s.AppendFormat(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		OutputDebugString(s);
		return;
	}

	SC_HANDLE handle = ::OpenService(scm, svc.name(), SERVICE_ALL_ACCESS);
	if (handle == NULL)
	{
		_tprintf(_T("OpenService failed, please manual to Auto Delay\n"));
		s.Empty();
		s.AppendFormat(_T("OpenService() for %s failed: %d\n"), svc.name(), ::GetLastError());
		OutputDebugString(s);
		return;
	}

	SERVICE_DELAYED_AUTO_START_INFO info = { true };
	BOOL ret = ::ChangeServiceConfig2(handle, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &info);
	if (ret == TRUE)
	{
		_tprintf(_T("Service change to Auto Delay successful\n"));
		s.Empty();
		s.AppendFormat(_T("ChangeServiceConfig2() for %s successful\n"), svc.name());
		OutputDebugString(s);
	}
	else
	{
		_tprintf(_T("ChangeServiceConfig2 failed, please manual to Auto Delay\n"));
		s.Empty();
		s.AppendFormat(_T("ChangeServiceConfig2() for %s failed: %d\n"), svc.name(), ::GetLastError());
		OutputDebugString(s);
	}

	return ;
}

inline
bool
delete_service(service_base const& svc)
{
	service_handle scm = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
	if (!scm) 
	{
		_tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
		return false;
	}

	service_handle h = ::OpenService(scm, svc.name(),
		SERVICE_QUERY_STATUS |
		SERVICE_STOP |
		DELETE);

	if (!h) 
	{
		_tprintf(_T("Couldn't open service control manager: %d\n"), ::GetLastError());
		return false;
	}

	SERVICE_STATUS svstat = {};
	if (::ControlService(h, SERVICE_CONTROL_STOP, &svstat)) 
	{
		_tprintf(_T("Stoping service %s\n"), svc.name().GetString());

		while (::QueryServiceStatus(h, &svstat)) 
		{
			if (svstat.dwCurrentState != SERVICE_STOP_PENDING)
			{
				break;
			}
		}

		if (svstat.dwCurrentState != SERVICE_STOPPED) 
		{
			_tprintf(_T("Failed to stop the service\n"));
		}
		else 
		{
			_tprintf(_T("Service stopped\n"));
		}
	}
	else 
	{
		_tprintf(_T("Didn't control service: %d\n"), ::GetLastError());
	}

	if (!::DeleteService(h))
	{
		_tprintf(_T("Failed to delete the service: %d\n"), GetLastError());
		return false;
	}

	return true;
}

inline 
void unstall_service(service_base const& svc)
{
	_tprintf(_T("Uninstalling service\n"));

	CString s;
	s.AppendFormat(_T("Uninstalling %s service\n"), svc.name());
	OutputDebugString(s);

	if (!delete_service(svc)) {
		s.Empty();
		s.AppendFormat(_T("Couldn't uninstall %s service: %d\n"), svc.name(), ::GetLastError());
		OutputDebugString(s);
		return;
	}

	_tprintf(_T("Uninstall successful\n"));
	s.Empty();
	s.AppendFormat(_T("Uninstall %s service successful\n"), svc.name());
	OutputDebugString(s);
	return;
}

_GURU_END

#endif /* _GURU_SERVICE_INSTALLER_H_ */