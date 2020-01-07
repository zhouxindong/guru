#pragma once
#ifndef _GURU_UDP_RECVER_H_
#define _GURU_UDP_RECVER_H_

#include "../gvals.h"
#include "recver_socket.h"

_GURU_BEGIN

/*
** udp receiver
*/
template <size_t N = 65535>
class udp_recver : public recver_socket<N>
{
	using string = std::string;

	typedef udp_recver<N> type;
	typedef recver_socket<N> base_type;
	typedef typename base_type::base_type root_type;

public:
	udp_recver() noexcept {}
	udp_recver(string ip, uint16_t port) noexcept;
};

template<size_t N>
inline 
udp_recver<N>::udp_recver(string ip, uint16_t port) noexcept
	: base_type(ip, port, ip, 0)
{
}

_GURU_END

#endif /* _GURU_UDP_RECVER_H_ */