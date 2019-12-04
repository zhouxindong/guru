#pragma once
#ifndef _GURU_LOG_ITEM_GENERATOR_H_
#define _GURU_LOG_ITEM_GENERATOR_H_

#include "../gvals.h"
#include <vector>
#include "log_item.h"
#include <string>

_GURU_BEGIN

std::vector<log_item> 
generate_log_item(size_t count, std::string& suffix = std::string()) noexcept
{
	if (count == 0)
		return std::vector<log_item>();

	std::vector<log_item> v;
	v.reserve(count);

	for (size_t i = 0; i < count; ++i)
	{
		int r = rand() % 6;
		switch (r)
		{
		case 0:
			v.push_back(LOG(TRACE) << log_level(_LOG_TRACE) << i << suffix);
			break;

		case 1:
			v.push_back(LOG(DEBUG) << log_level(_LOG_DEBUG) << i << suffix);
			break;

		case 2:
			v.push_back(LOG(INFO) << log_level(_LOG_INFO) << i << suffix);
			break;

		case 3:
			v.push_back(LOG(WARN) << log_level(_LOG_WARN) << i << suffix);
			break;

		case 4:
			v.push_back(LOG(ERROR) << log_level(_LOG_ERROR) << i << suffix);
			break;

		case 5:
			v.push_back(LOG(FATAL) << log_level(_LOG_FATAL) << i << suffix);
			break;
		}
	}

	return v;
}

_GURU_END

#endif /* _GURU_LOG_ITEM_GENERATOR_H_ */