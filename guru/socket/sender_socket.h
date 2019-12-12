#pragma once
#ifndef _GURU_SENDER_SOCKET_H_
#define _GURU_SENDER_SOCKET_H_

#include "../gvals.h"
#include "base_socket.h"

_GURU_BEGIN

/*
** base class for sender
*/
template <typename _Fixable = null_fixable>
class sender_socket : public base_socket
{
	using string = std::string;
protected:
	typedef sender_socket<_Fixable> type;
	typedef base_socket base_type;

public:
	int send(char const* data, int len) noexcept; 

protected:
	_Fixable _fixable;

protected:
	sender_socket(string si, uint16_t sp, string di, uint16_t dp) noexcept;
};

template<typename _Fixable>
inline 
int 
sender_socket<_Fixable>::send(char const * data, int len) noexcept
{
	_fixable.prefix();
	int size = 0;
	try
	{
		size = ::sendto(_socket, data, len, 0, (SOCKADDR*)&_dst_addr, SOCK_ADDR_SIZE);
	}
	catch (...)
	{
	}
	_fixable.suffix();
	return size;
}

template<typename _Fixable>
inline
sender_socket<_Fixable>::sender_socket(string si, uint16_t sp, string di, uint16_t dp) noexcept
	: base_type(si, sp, di, dp)
{
}

_GURU_END

#endif /* _GURU_SENDER_SOCKET_H_ */

