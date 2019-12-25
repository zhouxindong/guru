#pragma once
#include "nut/service_base.h"
#include <WtsApi32.h>

#pragma comment(lib, "Wtsapi32.lib")

class test_service : public guru::service_base
{
public:
	test_service(test_service const&) = delete;
	test_service& operator=(test_service const&) = delete;
	test_service(test_service&&) = delete;
	test_service& operator=(test_service&&) = delete;

	test_service()
		: service_base(
			_T("TestService"),
			_T("Test Service#"),
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			SERVICE_ACCEPT_STOP)
	{
	}

private:
	void on_start(DWORD argc, TCHAR* argv[]) override
	{
		OutputDebugString(_T("Test Service on_start()"));
	}
	void on_stop() override
	{
		OutputDebugString(_T("Test Service on_stop()"));
	}
};