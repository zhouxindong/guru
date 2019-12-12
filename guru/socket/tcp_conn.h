#pragma once
#ifndef _GURU_TCP_CONN_H_
#define _GURU_TCP_CONN_H_

#include "../gvals.h"

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


private:
	SOCKET _socket;
	SOCKADDR_IN _addr;

	std::vector<uint8_t> _s_buf;

	zxx::signal<void(std::string)> _disconn_signal;
	zxx::signal<void(std::vector<uint8_t>)> _recved_signal;

	_Tx _trait_stream;

private:
	bool _prepare_send{ false };
	bool _running{ false };

};

_GURU_END

#endif /* _GURU_TCP_CONN_H_ */