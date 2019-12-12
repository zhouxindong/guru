#pragma once
#ifndef _GURU_TCP_BASE_H_
#define _GURU_TCP_BASE_H_

#include "../gvals.h"
#include <WinSock2.h>

#pragma warning(disable:4275 4251) // disable warning C4275, C4251
#pragma comment(lib, "Ws2_32.lib")

_GURU_BEGIN

/*
** check socket connect whether living
*/
inline
bool
is_alive(SOCKET& socket) noexcept
{
	::fd_set rfds;
	int res, read_res;
	char buf[20] = { '\0' };
	::timeval timeout = { 3,0 };
	int again_time = 0;
	FD_ZERO(&rfds);
	FD_SET(socket, &rfds);

	res = ::select(0, &rfds, NULL, NULL, &timeout);
	if (res > 0)
	{
		read_res = ::recv(socket, buf, sizeof(buf), 0);
		if (read_res < 0)
		{
			if (errno == EINTR) return true; // interrupted by the signal
			return false;
		}
		else if (read_res == 0)
		{
			return false;
		}
		else // connect normal
		{
			return true;
		}
	}
	else if (res == 0) // time out, connect normal
	{
		return true;
	}
	else if (ERROR == EINTR)
	{
		return true;
	}
	else // select error
	{
		return false;
	}
}

_GURU_END

#endif /* _GURU_TCP_BASE_H_ */