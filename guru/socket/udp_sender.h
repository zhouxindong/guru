#pragma once
#ifndef _GURU_UDP_SENDER_H_
#define _GURU_UDP_SENDER_H_

#include "../gvals.h"
#include "sender_socket.h"

_GURU_BEGIN

/*
** udp sender
*/
template <typename _Fixable = null_fixable>
class udp_sender : public sender_socket<_Fixable>
{
	using string = std::string;

	typedef udp_sender<_Fixable> type;
	typedef sender_socket<_Fixable> base_type;
	typedef typename base_type::base_type root_type;

public:
	udp_sender() noexcept {}
	udp_sender(string di, uint16_t dp, string si = "127.0.0.1", uint16_t sp = 0) noexcept;
};

template<typename _Fixable>
inline 
udp_sender<_Fixable>::udp_sender(string di, uint16_t dp, string si, uint16_t sp) noexcept
	: base_type(si, sp, di, dp)
{
}
_GURU_END

#endif /* _GURU_UDP_SENDER_H_ */


