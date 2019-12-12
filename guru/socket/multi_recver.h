#pragma once
#ifndef _GURU_MULTI_RECVER_H_
#define _GURU_MULTI_RECVER_H_

#include "../gvals.h"
#include "recver_socket.h"

_GURU_BEGIN

/*
** multicast receiver
*/
template <size_t N = 65535>
class multi_recver : public recver_socket<N>
{
	using string = std::string;

	typedef multi_recver<N> type;
	typedef recver_socket<N> base_type;
	typedef typename base_type::base_type root_type;

public:
	multi_recver(string ri, uint16_t rp, string si = "127.0.0.1") noexcept;

private:
	ip_mreq _group;
};

template<size_t N>
inline
multi_recver<N>::multi_recver(string ri, uint16_t rp, string si) noexcept
	: base_type(si, rp, ri, rp)
{
	if (!root_type::_ready) return;
	root_type::_ready = false;

	int reuse = 1;
	if (setsockopt(root_type::_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
		return;

	_group.imr_multiaddr.s_addr = inet_addr(ri.c_str());
	_group.imr_interface.s_addr = inet_addr(si.c_str());
	if (setsockopt(root_type::_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&_group, sizeof(_group)) < 0) 
		return;

	root_type::_ready = true;
}

_GURU_END

#endif /* _GURU_MULTI_RECVER_H_ */

