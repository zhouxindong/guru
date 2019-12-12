#pragma once
#ifndef _GURU_BROAD_SENDER_H_
#define _GURU_BROAD_SENDER_H_

#include "../gvals.h"
#include "sender_socket.h"
#include "../base_feature/null_fixable.h"
#include "niceer.h"

_GURU_BEGIN

/*
** broadcast sender
*/
template <typename _Fixable = null_fixable, size_t _TTL = 1>
class broad_sender : public sender_socket<_Fixable>
{
	using string = std::string;

	typedef broad_sender<_Fixable, _TTL> type;
	typedef sender_socket<_Fixable> base_type;
	typedef typename base_type::base_type root_type;

public:
	broad_sender(uint16_t bp, string si = "127.0.0.1", uint16_t sp = 0) noexcept;
};

template<typename _Fixable, size_t _TTL>
inline
broad_sender<_Fixable, _TTL>::broad_sender(uint16_t bp, string si, uint16_t sp) noexcept
	: base_type(si, sp, niceer().broad_ip(si), bp)
{
	if (!root_type::_ready) return;
	root_type::_ready = false;

	bool broad = true;
	if (setsockopt(root_type::_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broad, sizeof(broad)) < 0)
		return;
	int ttl = (int)_TTL;
	setsockopt(root_type::_socket, IPPROTO_IP, IP_TTL, (char*)&ttl, sizeof(ttl));

	root_type::_ready = true;
}

_GURU_END

#endif /* _GURU_BROAD_SENDER_H_ */

