#pragma once
#ifndef _GURU_MULTI_SENDER_H_
#define _GURU_MULTI_SENDER_H_

#include "../gvals.h"
#include "udp_sender.h"
#include "../base_feature/null_fixable.h"

_GURU_BEGIN

/*
** multicast sender
*/
template <typename _Fixable = null_fixable, size_t _TTL = 100>
class multi_sender : public sender_socket<_Fixable>
{
	using string = std::string;

	typedef multi_sender<_Fixable, _TTL> type;
	typedef sender_socket<_Fixable> base_type;
	typedef typename base_type::base_type root_type;

public:
	multi_sender(string di, uint16_t dp, string si = "127.0.0.1", uint16_t sp = 0) noexcept;

private:
	in_addr _ia;
};

template<typename _Fixable, size_t _TTL>
inline 
multi_sender<_Fixable, _TTL>::multi_sender(string di, uint16_t dp, string si, uint16_t sp) noexcept
	: base_type(si, sp, di, dp)
{
	if (!root_type::_ready) return;
	root_type::_ready = false;

	::inet_pton(AF_INET, root_type::_src_ip.c_str(), (void*)&_ia.s_addr);
	if (setsockopt(root_type::_socket, IPPROTO_IP, IP_MULTICAST_IF, (char*)&_ia, sizeof(_ia)) < 0)
		return;

	int ttl = (int)_TTL;
	setsockopt(root_type::_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));

	root_type::_ready = true;
}

_GURU_END

#endif /* _GURU_MULTI_SENDER_H_ */

