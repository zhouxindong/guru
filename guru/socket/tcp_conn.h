#pragma once
#ifndef _GURU_TCP_CONN_H_
#define _GURU_TCP_CONN_H_

#include "../gvals.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include "../nut/signal.h"
#include "../stream/head_tail_stream.h"
#include "../stream/transparent_stream.h"
#include <sstream>

#pragma warning(disable:4275 4251) // disable warning C4275, C4251
#pragma comment(lib, "Ws2_32.lib")

_GURU_BEGIN

/*
** represent a tcp connection
** this connection can both send and receive data
** treat data received as stream, so _Tx to parse it
** N: receive buffer size
*/
template <
	typename _Trait_stream = transparent_stream<>,
	size_t N = 4096>
class tcp_conn
{
	using string = std::string;

public:
	typedef tcp_conn<_Trait_stream, N> type;
	typedef _Trait_stream trait_stream_type;
	static constexpr size_t Recv_Buf_Size = N;

public:
	template <typename... Types>	// Types... used for diff trait stream
	tcp_conn(SOCKET s, SOCKADDR_IN a, Types... args) noexcept;

	~tcp_conn() noexcept;

public:

public:
	void start() noexcept;	// connect exec body
	void stop() noexcept;	// stop connect

public:
	/*
	** if want to send data to client
	** 1. call send_buf() to get buffer
	** 2. fill buffer
	** 3. call enable_parpare_send()
	*/
	void enable_prepare_send() noexcept;

	signal<void(string)>& disconn_signal() noexcept;
	signal<void(tcp_conn<_Trait_stream, N>&, std::vector<uint8_t>)>& conn_recved_data_signal() noexcept;

	//_Trait_stream& trait_stream() noexcept;
	std::vector<uint8_t>& send_buf() noexcept;
	string conn_id() noexcept;
	SOCKET& socket() noexcept;
	SOCKADDR_IN& addr() noexcept;

private:
	SOCKET _socket;
	SOCKADDR_IN _addr;

	std::vector<uint8_t> _s_buf;						// send data buffer
	bool _prepare_send{ false };

	signal<void(std::string)> _disconn_signal;			// emit when disconnect
	signal<void(std::vector<uint8_t>)> _recved_signal;	// connect to Trait_stream's data src signal
	signal<void(tcp_conn<_Trait_stream, N>&, std::vector<uint8_t>)> _conn_recved_data_signal;

	_Trait_stream _trait_stream;						// data protocol

private:
	bool _running{ false };

private:
	void _trait_matched(std::vector<uint8_t> v) noexcept;
};

template<typename _Trait_stream, size_t N>
template<typename... Types>
inline
tcp_conn<_Trait_stream, N>::tcp_conn(SOCKET s, SOCKADDR_IN a, Types... args) noexcept
	: _socket(s), _addr(a), _trait_stream{ std::forward<Types>(args)... }
{
	_trait_stream.set_src_signal(_recved_signal);
	_trait_stream.matched_signal().connect(this, &tcp_conn::_trait_matched);
}

template<typename _Trait_stream, size_t N>
inline
tcp_conn<_Trait_stream, N>::~tcp_conn() noexcept
{
	_disconn_signal.disconnect_all();
	_recved_signal.disconnect_all();
	_conn_recved_data_signal.disconnect_all();

	_running = false;
}

template<typename _Trait_stream, size_t N>
inline 
void 
tcp_conn<_Trait_stream, N>::start() noexcept
{
	_running = true;

	::fd_set fdset;
	::timeval timeout = { 1,0 };
	int size = 0;
	int ret = 0;
	char buf[N];

	while (_running)
	{
		try
		{
			if (_prepare_send)	// send data
			{
				size = ::send(_socket, (const char*)_s_buf.data(), (int)_s_buf.size(), 0);
				_prepare_send = false;
				if (size < 0)
				{
					break;
				}
			}

			FD_ZERO(&fdset);
			FD_SET(_socket, &fdset);
			ret = ::select(0, &fdset, NULL, NULL, &timeout);
			if (ret <= 0)
			{
				::Sleep(1);
			}
			else if (FD_ISSET(_socket, &fdset))
			{
				size = ::recv(_socket, buf, N, 0);
				if (size > 0)
				{
					vector<uint8_t> rv(buf, buf + size);
					_recved_signal(rv);	// emit signal
				}
				else
				{
					break;
				}
			}
		}
		catch (...)
		{
			break;
		}
	}

	_disconn_signal(conn_id());
	closesocket(_socket);
}

template<typename _Trait_stream, size_t N>
inline 
void 
tcp_conn<_Trait_stream, N>::stop() noexcept
{
	_running = false;
}

template<typename _Trait_stream, size_t N>
inline 
signal<void(std::string)>& 
tcp_conn<_Trait_stream, N>::disconn_signal() noexcept
{
	return _disconn_signal;
}

template<typename _Trait_stream, size_t N>
inline 
signal<void(tcp_conn<_Trait_stream, N>&, std::vector<uint8_t>)>& 
tcp_conn<_Trait_stream, N>::conn_recved_data_signal() noexcept
{
	return _conn_recved_data_signal;
}

template<typename _Trait_stream, size_t N>
inline
void
tcp_conn<_Trait_stream, N>::enable_prepare_send() noexcept
{
	_prepare_send = true;
}

//template<typename _Trait_stream, size_t N>
//inline 
//_Trait_stream & 
//tcp_conn<_Trait_stream, N>::trait_stream() noexcept
//{
//	return _trait_stream;
//}

template<typename _Trait_stream, size_t N>
inline 
std::vector<uint8_t>& 
tcp_conn<_Trait_stream, N>::send_buf() noexcept
{
	return _s_buf;
}

template<typename _Trait_stream, size_t N>
inline 
std::string 
tcp_conn<_Trait_stream, N>::conn_id() noexcept
{
	std::ostringstream oss;
	oss << (int)_addr.sin_addr.S_un.S_un_b.s_b1 << '.'
		<< (int)_addr.sin_addr.S_un.S_un_b.s_b2 << '.'
		<< (int)_addr.sin_addr.S_un.S_un_b.s_b3 << '.'
		<< (int)_addr.sin_addr.S_un.S_un_b.s_b4 << ':'
		<< _addr.sin_port;
	return oss.str();
}

template<typename _Trait_stream, size_t N>
inline 
SOCKET & 
tcp_conn<_Trait_stream, N>::socket() noexcept
{
	return _socket;
}

template<typename _Trait_stream, size_t N>
inline 
SOCKADDR_IN & 
tcp_conn<_Trait_stream, N>::addr() noexcept
{
	return _addr;
}

template<typename _Trait_stream, size_t N>
inline
void 
tcp_conn<_Trait_stream, N>::_trait_matched(std::vector<uint8_t> v) noexcept
{
	_conn_recved_data_signal(*this, v);
}

_GURU_END

#endif /* _GURU_TCP_CONN_H_ */

