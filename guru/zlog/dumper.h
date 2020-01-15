#pragma once
#ifndef _GURU_ZLOG_DUMPER_H_
#define _GURU_ZLOG_DUMPER_H_

#include "../gvals.h"
#include "log_item.h"
#include "../dump/stack_tracer.h"
#include "../dump/mem_tracer.h"

_GURU_BEGIN

template <typename Logger>
inline void dump_stack_trace(Logger& logger)
{
	log_item li = LOG(TRACE) << stack_tracer::stack_trace();
	logger.log(li);
}

template <typename Logger>
inline void dump_exception_msg(Logger& logger, std::exception& e)
{
	log_item li = LOG(DEBUG) << e.what();
	logger.log(li);
}

template <typename Logger>
inline void dump_memory(Logger& logger)
{
	log_item li = LOG(TRACE) << '\n' << dump_mem_tracer();
	logger.log(li);
}

_GURU_END

#endif // _GURU_ZLOG_DUMPER_H_
