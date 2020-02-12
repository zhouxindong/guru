#pragma once
#ifndef _GURU_RECVER_SOCKET_H_
#define _GURU_RECVER_SOCKET_H_

#include "../gvals.h"
#include "base_socket.h"
#include <vector>
#include <functional>

_GURU_BEGIN

/*
** base class for recver
*/
template <size_t N = 65535>
class recver_socket : public base_socket
{
	using string = std::string;

protected:
	typedef recver_socket<N> type;
	typedef base_socket base_type;

public:
	void start() noexcept;
	void stop() noexcept;
	void add_listen(std::function<void(std::vector<char>)> listen) noexcept;

protected:
	recver_socket() noexcept {}
	recver_socket(string si, uint16_t sp, string di, uint16_t dp) noexcept;
	~recver_socket() noexcept;

protected:
	bool _running{ false };
	char _buf[N];
	std::vector<std::function<void(std::vector<char>)>> _slots;
};

template<size_t N>
inline 
void 
recver_socket<N>::start() noexcept
{
	if (!_ready) return;
	_running = true;

	::fd_set fdset;
	::timeval timeout = { 1,0 };
	int size = 0;

	while (_running)
	{
		try
		{
			FD_ZERO(&fdset);
			FD_SET(_socket, &fdset);

			int ret = ::select(0, &fdset, NULL, NULL, &timeout);
			if (ret <= 0)
			{
				::Sleep(1);
			}
			else if (FD_ISSET(_socket, &fdset))
			{
				size = ::recv(_socket, _buf, N, 0);
				if (size > 0)
				{
					std::vector<char> rv(_buf, _buf + size);
					for_each(_slots.cbegin(), _slots.cend(),
						[&](auto slot) {
						slot(rv);
					});
				}
			}
		}
		catch (...)
		{
		}
	}
}

template<size_t N>
inline 
void 
recver_socket<N>::stop() noexcept
{
	_running = false;
}

template<size_t N>
inline
void 
recver_socket<N>::add_listen(std::function<void(std::vector<char>)> listen) noexcept
{
	_slots.push_back(listen);
}

template<size_t N>
inline 
recver_socket<N>::recver_socket(string si, uint16_t sp, string di, uint16_t dp) noexcept
	: base_type(si, sp, di, dp)
{
}

template<size_t N>
inline 
recver_socket<N>::~recver_socket() noexcept
{
	_running = false;
}

_GURU_END

#endif /* _GURU_RECVER_SOCKET_H_ */

