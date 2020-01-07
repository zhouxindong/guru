#pragma once
#ifndef _GURU_BASE_SOCKET_H_
#define _GURU_BASE_SOCKET_H_

#include "../gvals.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "../base_feature/noncopyable.h"
#include "../base_feature/nonassignable.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4275 4251) // disable warning C4275, C4251

_GURU_BEGIN

static constexpr int SOCK_ADDR_SIZE = sizeof(SOCKADDR);
static constexpr int SOCK_ADDR_IN_SIZE = sizeof(SOCKADDR_IN);

inline
void
init_sockaddr_in(SOCKADDR_IN& addr, std::string ip, uint16_t port) noexcept
{
	::memset(&addr, 0, SOCK_ADDR_IN_SIZE);
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	::inet_pton(AF_INET, ip.c_str(), (void*)&addr.sin_addr.s_addr);
}

inline
bool
init_WSA() noexcept
{
	WORD ver_req = MAKEWORD(2, 2);
	WSADATA wsa;
	int err = ::WSAStartup(ver_req, &wsa);
	if (err != 0) return false;

	if ((LOBYTE(wsa.wVersion) != 2) || HIBYTE(wsa.wVersion != 2)) 
	{
		::WSACleanup();
		return false;
	}
	return true;
}

inline
bool
init_socket(SOCKET& socket, int type = SOCK_DGRAM, int protocol = IPPROTO_UDP) noexcept
{
	socket = ::socket(AF_INET, type, protocol);
	if (INVALID_SOCKET == socket)
	{
		WSACleanup();
		return false;
	}
	return true;
}

inline
bool
bind_socket(SOCKET& socket, const sockaddr* name) noexcept
{
	if (::bind(socket, name, SOCK_ADDR_SIZE) == SOCKET_ERROR)
	{
		closesocket(socket);
		WSACleanup();
		return false;
	}
	return true;
}

/*
** socket base class
*/
class base_socket : public noncopyable, public nonassignable
{
public:
	void src_ip(std::string const& s)
	{
		_src_ip = s;
	}

	void src_port(uint16_t v)
	{
		_src_port = v;
	}

	void dst_ip(std::string const& s)
	{
		_dst_ip = s;
	}

	void dst_port(uint16_t v)
	{
		_dst_port = v;
	}

	const bool get_ready() const
	{
		return _ready;
	}

protected:
	base_socket() noexcept {}
	base_socket(std::string si, uint16_t sp, std::string di, uint16_t dp) noexcept;
	~base_socket() noexcept;

protected:
	bool _error{ false };
	bool _ready{ false };

protected:
	SOCKET _socket;
	SOCKADDR_IN _src_addr;
	SOCKADDR_IN _dst_addr;

protected:
	std::string _src_ip;
	uint16_t _src_port;
	std::string _dst_ip;
	uint16_t _dst_port;

public:
	void init();
};

inline
base_socket::base_socket(std::string si, uint16_t sp, std::string di, uint16_t dp) noexcept
	: _src_ip{ si }, _src_port{ sp }, _dst_ip{ di }, _dst_port{ dp }
{
	init();
}

inline
base_socket::~base_socket() noexcept
{
	if (_ready)
	{
		closesocket(_socket);
		WSACleanup();
	}
}

inline void base_socket::init()
{
	if (!init_WSA())
	{
		_error = true;
		return;
	}
	init_sockaddr_in(_dst_addr, _dst_ip, _dst_port);
	init_sockaddr_in(_src_addr, _src_ip, _src_port);
	if (!init_socket(_socket)) return;
	if (!bind_socket(_socket, (SOCKADDR*)&_src_addr)) return;

	_ready = true;
}

_GURU_END

#endif /* _GURU_BASE_SOCKET_H_ */