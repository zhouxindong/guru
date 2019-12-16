#pragma once
#ifndef _GURU_TCP_BASE_H_
#define _GURU_TCP_BASE_H_

#include "../gvals.h"
#include <WinSock2.h>
#include <map>
#include <future>
#include "tcp_conn.h"
#include <iterator>
#include <memory>
#include "../stream/transparent_stream.h"
#include "base_socket.h"

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

/*
** base class for TCP
** main function: observer for received data
*/
template <
	typename _Trait_stream = transparent_stream<>,
	size_t N = 4096>
class tcp_base
{
public:
	typedef tcp_base<_Trait_stream, N> type;
	typedef _Trait_stream trait_stream_type;
	static constexpr size_t Buf_Size = N;

public:
	tcp_base(std::string ip, uint16_t port) noexcept;
	~tcp_base() noexcept;

public:
	virtual void stop() noexcept;
	void add_listen(std::function<void(tcp_conn<_Trait_stream, N>&, std::vector<uint8_t>)> f) noexcept;

protected:
	// received data through connection
	void _recved_data(tcp_conn<_Trait_stream, N>& conn, std::vector<uint8_t> v) noexcept;

protected:
	bool _running{ false };	// true for alive
	bool _ready{ false };
	std::mutex _mutex;
	std::vector<std::function<void(tcp_conn<_Trait_stream, N>&, std::vector<uint8_t>)>> _slots;	// all observers

protected:
	std::string _srv_ip;	// TCP server IP
	uint16_t _srv_port;		// TCP server port
	SOCKADDR_IN _srv_addr;
	SOCKET _socket;
};

template<typename _Trait_stream, size_t N>
inline 
tcp_base<_Trait_stream, N>::tcp_base(std::string ip, uint16_t port) noexcept
	: _srv_ip(ip), _srv_port(port)
{
	if (!init_WSA()) return;
	init_sockaddr_in(_srv_addr, _srv_ip, _srv_port);
	if (!init_socket(_socket, SOCK_STREAM, IPPROTO_TCP)) return;
	_ready = true;
}

template<typename _Trait_stream, size_t N>
inline
tcp_base<_Trait_stream, N>::~tcp_base() noexcept
{
	_running = false;
}

template<typename _Trait_stream, size_t N>
inline 
void 
tcp_base<_Trait_stream, N>::stop() noexcept
{
	_running = false;
}

template<typename _Trait_stream, size_t N>
inline 
void 
tcp_base<_Trait_stream, N>::add_listen(std::function<void(tcp_conn<_Trait_stream, N>&, std::vector<uint8_t>)> f) noexcept
{
	std::lock_guard<std::mutex> locker(_mutex);
	_slots.push_back(f);
}

template<typename _Trait_stream, size_t N>
inline 
void 
tcp_base<_Trait_stream, N>::_recved_data(tcp_conn<_Trait_stream, N>& conn, std::vector<uint8_t> v) noexcept
{
	std::lock_guard<std::mutex> locker(_mutex);
	if (_slots.empty()) return;
	for_each(_slots.cbegin(), _slots.cend(), [&](auto& f) {
		f(conn, v);
	});
}

_GURU_END

#endif /* _GURU_TCP_BASE_H_ */