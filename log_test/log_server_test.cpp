#include "stdafx.h"

#include "zlog/log_service.h"
#include "zlog/config_logger.h"

using namespace std;
using namespace guru;

//int main()
//{
//	log_server ls("10.16.2.55", 8923, "C:\\zlog", "server1");
//	ls.start();
//
//	config_log logger("log.config.xml");
//
//	for (int i = 0; i < 1000; ++i)
//	{
//		log_item li = LOG(TRACE) << "this is a trace";
//		logger.log(li);
//		li = LOG(DEBUG) << "this is a debug";
//		logger.log(li);
//		li = LOG(INFO) << "this is a info";
//		logger.log(li);
//		li = LOG(WARN) << "this is a warn";
//		logger.log(li);
//		li = LOG(ERROR) << "this is a error";
//		logger.log(li);
//		li = LOG(FATAL) << "this is a fatal";
//		logger.log(li);
//	}
//
//	system("pause");
//	return 0;
//}