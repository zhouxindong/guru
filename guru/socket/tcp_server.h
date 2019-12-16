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

public:

	template <typename... Types>
	void start(Types... args) noexcept;

	/*
	** send data to all clients
	*/
	template <typename _Cont>
	void send(typename _Cont::const_iterator begin, typename _Cont::const_iterator end) noexcept;

	std::map<std::string, std::shared_ptr<tcp_conn<_Trait_stream, N>>>& clients() noexcept;

public:
	/*
	** call back when tcp server started
	** return false means you want stop it
	*/
	virtual bool on_started() noexcept { return true; }
	/*
	** call back when tcp server accept a new client connect
	** call clients() here you can get exists all connections
	** return false means you don't received this client connect
	*/
	virtual bool on_new_client(std::shared_ptr<tcp_conn<_Trait_stream, N>> conn) noexcept { return true; }

	virtual void on_disconn_client(std::string conn_id) noexcept {}

private:
	void _clear_clients() noexcept;	// stop all clients 
	void _client_disconn(std::string conn_id) noexcept;
	static void _start_conn_proc(std::shared_ptr<tcp_conn<_Trait_stream, N>> c) noexcept;

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
	if (!bind_socket(base_type::_socket, (SOCKADDR*)&_srv_addr)) return;

	if (::listen(base_type::_socket, _MAX_CONN) < 0)
	{
		closesocket(base_type::_socket);
		WSACleanup();
		return;
	}

	base_type::_ready = true;
}

template<typename _Trait_stream, size_t N>
inline 
std::map<std::string, std::shared_ptr<tcp_conn<_Trait_stream, N>>>& 
tcp_server<_Trait_stream, N>::clients() noexcept
{
	return _clients;
}

template<typename _Trait_stream, size_t N>
inline 
void 
tcp_server<_Trait_stream, N>::_clear_clients() noexcept
{
	std::lock_guard<std::mutex> locker(base_type::_mutex);
	for_each(_clients.cbegin(), _clients.cend(),
		[](std::pair<std::string, std::shared_ptr<tcp_conn<_Trait_stream, N>>> p)
	{
		p.second->stop();
	});
}

template<typename _Trait_stream, size_t N>
inline
void 
tcp_server<_Trait_stream, N>::_client_disconn(std::string conn_id) noexcept
{
	{
		std::lock_guard<std::mutex> locker(base_type::_mutex);
		if (_clients.size() == 0) return;
		auto iter = _clients.find(conn_id);
		if (iter == _clients.end()) return;
		_clients.erase(conn_id);
	}
	on_disconn_client(conn_id);
}

template<typename _Trait_stream, size_t N>
inline
void 
tcp_server<_Trait_stream, N>::_start_conn_proc(std::shared_ptr<tcp_conn<_Trait_stream, N>> c) noexcept
{
	c->start();
}

template<typename _Trait_stream, size_t N>
template<typename ...Types>
inline
void 
tcp_server<_Trait_stream, N>::start(Types ...args) noexcept
{
	if (!base_type::_ready) return;

	base_type::_running = true;
	::fd_set fdset;
	::timeval timeout = { 1,0 };
	int len = sizeof(SOCKADDR);
	while (base_type::_running)
	{
		if (!on_started()) break;

		FD_ZERO(&fdset);
		FD_SET(base_type::_socket, &fdset);

		if (::select(0, &fdset, NULL, NULL, &timeout) <= 0)
		{
			Sleep(100);
		}
		else if (FD_ISSET(base_type::_socket, &fdset))
		{
			SOCKADDR_IN client_addr;
			SOCKET client = ::accept(base_type::_socket, (SOCKADDR*)&client_addr, &len);
			if (INVALID_SOCKET == client)
			{
				if (WSAEWOULDBLOCK == WSAGetLastError())
				{
					Sleep(100);
					continue;
				}
				else
				{
					break;
				}
			}
			else
			{
				shared_ptr<tcp_conn<_Trait_stream, N>> 
					conn(new tcp_conn<_Trait_stream, N>(client, client_addr, std::forward<Types>(args)...));
				if (!on_new_client(conn)) continue;

				conn->conn_recved_data_signal().connect(this, &tcp_server::_recved_data);
				conn->disconn_signal().connect(this, &tcp_server::_client_disconn);

				{
					std::lock_guard<std::mutex> locker(base_type::_mutex);
					_clients[conn->conn_id()] = conn;
				}

				// in async launch a new async would paused old async until the new one finisned
				//std::future<void> f(std::async(std::launch::async, _start_conn_proc, conn));
				std::thread t(_start_conn_proc, conn);
				t.detach();
			}
		}
	}
	_clear_clients();
}

template<typename _Trait_stream, size_t N>
template<typename _Cont>
inline 
void 
tcp_server<_Trait_stream, N>::send(typename _Cont::const_iterator begin, typename _Cont::const_iterator end) noexcept
{
	typename iterator_traits<decltype(begin)>::difference_type size = distance(begin, end);
	if (size <= 0) return;

	std::map<std::string, std::shared_ptr<tcp_conn<_Trait_stream, N>>> tmp;
	{
		std::lock_guard<std::mutex> locker(base_type::_mutex);
		tmp = _clients;
	}

	for_each(tmp.cbegin(), tmp.cend(), [&](auto& pair)
	{
		std::vector<uint8_t>& buf = pair.second->send_buf();
		buf.resize(size);
		copy(begin, end, buf.begin());
		pair.second->enable_prepare_send();
	});
}

_GURU_END

#endif /* _GURU_TCP_SERVER_H_ */

