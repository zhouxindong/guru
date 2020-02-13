// zlog_service.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "nut/service_installer.h"
#include "zlog/log_service.h"

using namespace guru;

int main(int argc, char* argv[])
{
	log_service service;

	OutputDebugString(_T("main()"));

	if (argc > 1) {
		if (strcmp(argv[1], ("install")) == 0) {
			guru::install_service(service);
			return 0;
		}

		if (strcmp(argv[1], ("uninstall")) == 0) {
			guru::unstall_service(service);
			return 0;
		}

		_tprintf(_T("Invalid argument\n"));
		return -1;
	}

	service.run();
	OutputDebugString(_T("end of main"));
	return 0;
}

