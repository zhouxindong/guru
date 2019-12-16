#pragma once
#ifndef _GURU_TCP_CLIENT_H_
#define _GURU_TCP_CLIENT_H_

#include "../gvals.h"
#include "../stream/head_tail_stream.h"
#include "../stream/transparent_stream.h"
#include "tcp_base.h"
#include "base_socket.h"

_GURU_BEGIN

/*
** TCP client
*/
template <
	typename _Trait_stream = transparent_stream<>,
	size_t N = 4096>
class tcp_client : public tcp_base<_Trait_stream, N>
{
	using string = std::string;

public:
	typedef tcp_client<_Trait_stream, N> type;
	typedef tcp_base<_Trait_stream, N> base_type;

	typedef _Trait_stream trait_stream_type;
	static constexpr size_t Buf_Size = N;

public:
	tcp_client(string si, uint16_t sp, string li = "127.0.0.1") noexcept;
	~tcp_client() noexcept;

public:
	template <typename... Types>
	void start(Types... args) noexcept;

	void stop() noexcept override;

	template <typename _Cont>
	void send(typename _Cont::const_iterator begin, typename _Cont::const_iterator end) noexcept;

public:
	/*
	** call back when connect to server
	** return false means you don't allow connect
	*/
	virtual bool on_connected() noexcept { return true; }
	/*
	** call back when disconnect
	*/
	virtual void on_disconnect() noexcept { }

private:
	std::string _loc_ip;
	SOCKADDR_IN _loc_addr;
	std::shared_ptr<tcp_conn<_Trait_stream, N>> _conn;
	bool _connected{ false };
};

template<typename _Trait_stream, size_t N>
inline
tcp_client<_Trait_stream, N>::tcp_client(string si, uint16_t sp, string li) noexcept
	: base_type(si, sp), _loc_ip{ li }
{
	if (!base_type::_ready) return;
	base_type::_ready = false;

	init_sockaddr_in(_loc_addr, _loc_ip, 0);
	if (!bind_socket(base_type::_socket, (SOCKADDR*)&_loc_addr)) return;

	base_type::_ready = true;
}

template<typename _Trait_stream, size_t N>
inline 
tcp_client<_Trait_stream, N>::~tcp_client() noexcept
{
	stop();
}

template<typename _Trait_stream, size_t N>
template<typename ...Types>
inline
void
tcp_client<_Trait_stream, N>::start(Types ...args) noexcept
{
	if (!base_type::_ready) return;
	base_type::_running = true;
	while (base_type::_running)
	{
		if (::connect(base_type::_socket, (SOCKADDR*)&_srv_addr, SOCK_ADDR_SIZE) == SOCKET_ERROR)
		{
			Sleep(1000);
		}
		else
		{
			if (!on_connected())
			{
				base_type::_running = false;
				break;
			}

			_connected = true;

			_conn.reset(new tcp_conn<_Trait_stream, N>(_socket, _loc_addr,
				std::forward<Types>(args)...));
			_conn->conn_recved_data_signal().connect(this, &tcp_client::_recved_data);
			_conn->start();

			_running = false;
			_connected = false;
		}
	}
	on_disconnect();
}

template<typename _Trait_stream, size_t N>
template<typename _Cont>
inline 
void 
tcp_client<_Trait_stream, N>::send(typename _Cont::const_iterator begin, typename _Cont::const_iterator end) noexcept
{
	if (!base_type::_running) return;

	typename iterator_traits<decltype(begin)>::difference_type size = distance(begin, end);
	if (size <= 0) return;

	std::vector<uint8_t>& buf = _conn->send_buf();
	buf.resize(size);
	copy(begin, end, buf.begin());
	_conn->enable_prepare_send();
}

template <typename _Trait_stream, size_t N>
inline 
void
tcp_client<_Trait_stream, N>::stop() noexcept
{
	if (!base_type::_running) return;
	if (!_connected)
	{
		base_type::_running = false;
		return;
	}
	_conn->stop();
}

_GURU_END

#endif /* _GURU_TCP_CLIENT_H_ */

