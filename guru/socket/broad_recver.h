#pragma once
#ifndef _GURU_BROAD_RECVER_H_
#define _GURU_BROAD_RECVER_H_

#include "../gvals.h"
#include "recver_socket.h"

_GURU_BEGIN

/*
** broadcast receiver
*/
template <size_t N = 65535>
class broad_recver : public recver_socket<N>
{
	using string = std::string;

	typedef broad_recver<N> type;
	typedef recver_socket<N> base_type;
	typedef typename base_type::base_type root_type;

public:
	broad_recver(uint16_t bp, string si = "127.0.0.1") noexcept;
};

template<size_t N>
inline 
broad_recver<N>::broad_recver(uint16_t bp, string si) noexcept
	: base_type(si, bp, si, bp)
{
	if (!root_type::_ready) return;
	root_type::_ready = false;

	bool broad = true;
	setsockopt(root_type::_socket, SOL_SOCKET, SO_BROADCAST, (char*)&broad, sizeof(broad));

	root_type::_ready = true;
}

_GURU_END

#endif /* _GURU_BROAD_RECVER_H_ */

