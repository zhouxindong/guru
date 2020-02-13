#pragma once
#ifndef _GURU_ZLOG_LOG_SERVICE_H_
#define _GURU_ZLOG_LOG_SERVICE_H_

#include "../gvals.h"
#include <future>
#include "../socket/udp_recver.h"
#include "../nut/service_base.h"
#include <WtsApi32.h>
#include "../grocery/path.h"
#include "../grocery/tinyxml2_wrap.h"
#include <memory>
#include <algorithm>

#pragma comment(lib, "Wtsapi32.lib")

_GURU_BEGIN

static std::string LOG_SERVICE_LOG		= "LOG";
static std::string LOG_SERVICE_PATH		= "PATH";
static std::string LOG_SERVICE_LOGGER	= "LOGGER";
static std::string LOG_SERVICE_NAME		= "NAME";
static std::string LOG_SERVICE_IP		= "IP";
static std::string LOG_SERVICE_PORT		= "PORT";

class log_server
{
public:
	log_server(
		std::string const& ip, uint16_t port,
		std::string const& log_path, std::string const& name)
		: _recver{ ip, port },
		_roll{ path::combine(log_path, name), name }
	{
		_recver.add_listen([&](std::vector<char> v) {
			_roll.stream() << std::string{ v.cbegin(), v.cend() };
			_roll.flush();
		});
	}

	~log_server()
	{
		_recver.stop();
	}

	log_server(log_server const&) = delete;
	log_server(log_server&&) = delete;
	log_server& operator = (log_server const&) = delete;
	log_server& operator = (log_server&&) = delete;

public:
	void start()
	{
		_f = std::async(std::launch::async, [&]() {
			_recver.start();
		});
	}

private:
	std::future<void> _f;
	udp_recver<4096> _recver;
	roll_file<> _roll;
};

class log_service : public service_base
{
public:
	log_service(log_service const&) = delete;
	log_service(log_service&&) = delete;

	log_service& operator = (log_service const&) = delete;
	log_service& operator = (log_service&&) = delete;

	log_service() : service_base(
		_T("z-logger"),
		_T("HJYD Logger (zlog.v1)"),
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		SERVICE_ACCEPT_STOP,
		true)
	{
	}

private:
	void on_start(DWORD argc, TCHAR* argv[]) override
	{
		OutputDebugString(_T("Logger HJYD (zlogv1) on_start()"));
		path::cwd(path::app_path());
		tinyxml2_wrap xml(_config_file.c_str());		

		auto p = xml.text_attribute(xml.root(), LOG_SERVICE_PATH.c_str());
		if (p.first) _path = p.second;
		OutputDebugStringA(p.second.c_str());

		auto l = xml.all(LOG_SERVICE_LOGGER.c_str(), LOG_SERVICE_LOG.c_str());
		for (auto g : l)
		{
			auto name = xml.text_attribute(g, LOG_SERVICE_NAME.c_str());
			if (!name.first) continue;
			OutputDebugStringA(name.second.c_str());

			auto ip = xml.text_attribute(g, LOG_SERVICE_IP.c_str());
			if (!ip.first) continue;
			OutputDebugStringA(ip.second.c_str());

			auto port = xml.int32_attribute(g, LOG_SERVICE_PORT.c_str());
			if (!port.first) continue;
			std::ostringstream oss;
			oss << port.second;
			OutputDebugStringA(oss.str().c_str());

			_servers.push_back(std::shared_ptr<log_server>{
				new log_server{
				ip.second, (uint16_t)port.second, _path, name.second
				}});
		}

		for_each(_servers.cbegin(), _servers.cend(), [](auto svr) {
			svr->start();
		});
	}

	void on_stop() override
	{
		OutputDebugString(_T("Logger HJYD (zlogv1) on_stop()"));
		_servers.clear();
	}

private:
	std::string _path{ "C:\\zlog" };
	std::string _config_file{ "log.service.xml" };
	std::vector<std::shared_ptr<log_server>> _servers;
};

_GURU_END

#endif // _GURU_ZLOG_LOG_SERVICE_H_