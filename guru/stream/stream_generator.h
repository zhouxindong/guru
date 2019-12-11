#pragma once

#ifndef _GURU_STREAM_GENERATOR_H_
#define _GURU_STREAM_GENERATOR_H_

#include "../gvals.h"
#include "../nut/signal.h"
#include <mutex>

#pragma warning(disable:4275 4251) // disable warning C4275, C4251

_GURU_BEGIN

/*
** trait byte stream generator
** _Volumn: size of body (not include trait bytes)
** _Interval: generate time interval (ms)
*/
template <size_t _Volumn, size_t _Interval = 1000>
class stream_generator
{
public:
	stream_generator() noexcept;
	~stream_generator() noexcept;

public:
	void set_head(std::vector<uint8_t> const& v) noexcept;
	void set_tail(std::vector<uint8_t> const& v) noexcept;

	void start() noexcept;
	void stop() noexcept;

	guru::signal<void(std::vector<uint8_t>)>& signal() noexcept;	// new block data generated

protected:
	// override it if you want to do something before signal emit
	// return true means emit signal, false not
	virtual bool _before_emit() noexcept { return true; }

protected:
	guru::signal<void(std::vector<uint8_t>)> _signal;	// data generated signal

	std::vector<uint8_t> _head;					// head trait bytes
	std::vector<uint8_t> _tail;					// tail trait bytes
	std::vector<uint8_t> _buf;					// whole block bytes
	std::vector<uint8_t> _body;					// body bytes

	std::mutex _mutex;
	bool _running{ false };

private:
	void _rebuild_buf() noexcept;	// after set head or tail, rebuild whole buffer
	void _fill_body() noexcept;		// fill random data of body buffer
};

template<size_t _Volumn, size_t _Interval>
inline 
stream_generator<_Volumn, _Interval>::stream_generator() noexcept
{
	_buf.resize(_Volumn);
	_body.resize(_Volumn);
}

template<size_t _Volumn, size_t _Interval>
inline 
stream_generator<_Volumn, _Interval>::~stream_generator() noexcept
{
	stop();
}

template<size_t _Volumn, size_t _Interval>
inline 
void 
stream_generator<_Volumn, _Interval>::set_head(std::vector<uint8_t> const & v) noexcept
{
	{
		std::lock_guard<std::mutex> locker(_mutex);
		_head = v;
	}
	_rebuild_buf();
}

template<size_t _Volumn, size_t _Interval>
inline 
void 
stream_generator<_Volumn, _Interval>::set_tail(std::vector<uint8_t> const & v) noexcept
{
	{
		std::lock_guard<std::mutex> locker(_mutex);
		_tail = v;
	}
	_rebuild_buf();
}

template<size_t _Volumn, size_t _Interval>
inline 
void 
stream_generator<_Volumn, _Interval>::start() noexcept
{
	_running = true;
	while (_running)
	{
		{
			std::lock_guard<std::mutex> locker(_mutex);
			_fill_body();
			typename std::vector<uint8_t>::iterator it = _buf.begin();
			advance(it, _head.size());
			copy(_body.cbegin(), _body.cend(), it);
			if (_before_emit())
				_signal(_buf);
		}
		Sleep((DWORD)_Interval);
	}
}

template<size_t _Volumn, size_t _Interval>
inline
void 
stream_generator<_Volumn, _Interval>::stop() noexcept
{
	_running = false;
}

template<size_t _Volumn, size_t _Interval>
inline 
guru::signal<void(std::vector<uint8_t>)>& stream_generator<_Volumn, _Interval>::signal() noexcept
{
	return _signal;
}

template<size_t _Volumn, size_t _Interval>
inline 
void 
stream_generator<_Volumn, _Interval>::_rebuild_buf() noexcept
{
	std::lock_guard<std::mutex> locker(_mutex);
	_buf.resize(_head.size() + _tail.size() + _Volumn);
	copy(_head.cbegin(), _head.cend(), _buf.begin());
	typename std::vector<uint8_t>::iterator it = _buf.begin();
	advance(it, _head.size() + _Volumn);
	copy(_tail.cbegin(), _tail.cend(), it);
}

template<size_t _Volumn, size_t _Interval>
inline 
void 
stream_generator<_Volumn, _Interval>::_fill_body() noexcept
{
	for (int i = 0; i < _Volumn; ++i)
	{
		_body[i] = rand() % 255;
	}
}

_GURU_END

#endif /* _GURU_STREAM_GENERATOR_H_ */


