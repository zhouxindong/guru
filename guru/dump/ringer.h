#pragma once
#ifndef _GURU_RINGER_H_
#define _GURU_RINGER_H_

#include "../gvals.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

#pragma comment (lib,"psapi.lib")

_GURU_BEGIN

inline
bool 
promote_privilege() noexcept
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process.
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	// Get the LUID for the EmptyWorkingSet privilege.
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1; // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the EmptyWorkingSet privilege for this process.
	return AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
}

/*
** memory pack
*/
inline 
bool
empty_all_set() noexcept
{
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (SnapShot == NULL)
	{
		return false;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	BOOL Status = Process32First(SnapShot, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			SetProcessWorkingSetSize(hProcess, -1, -1);
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		Status = Process32Next(SnapShot, &ProcessInfo);
	}
	return true;
}

_GURU_END

#endif /* _GURU_RINGER_H_ */