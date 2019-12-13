#pragma once
#ifndef _GURU_TCP_SERVER_H_
#define _GURU_TCP_SERVER_H_

#include "../gvals.h"
#include "tcp_base.h"

_GURU_BEGIN

/*
** TCP server
*/
template <
	typename _Trait_stream = transparent_stream<>,
	size_t N = 4096>
class tcp_server : public tcp_base<_Trait_stream, N>
{
public:
	typedef tcp_server<_Trait_stream, N> type;
	typedef tcp_base<_Trait_stream, N> base_type;
	typedef _Trait_stream trait_stream_type;
	static constexpr size_t Buf_Size = N;

public:
	tcp_server(std::string ip, uint16_t port) noexcept;

private:
	std::map<std::string, std::shared_ptr<tcp_conn<_Trait_stream, N>>> _clients;
	static constexpr int _MAX_CONN = 5;
};

template<typename _Trait_stream, size_t N>
inline
tcp_server<_Trait_stream, N>::tcp_server(std::string ip, uint16_t port) noexcept
	: base_type(ip, port)
{
	if (!base_type::_ready) return;
	if (!bind_socket(base_type::_socket, (SOCKADDR*)&base_type::_srv_addr)) return;

	if (::listen(base_type::_socket, _MAX_CONN) < 0)
	{
		closesocket(base_type::_socket);
		WSACleanup();
		return;
	}

	base_type::_ready = true;
}

_GURU_END

#endif /* _GURU_TCP_SERVER_H_ */