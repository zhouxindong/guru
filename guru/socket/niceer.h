#pragma once
#ifndef _GURU_NICEER_H_
#define _GURU_NICEER_H_

#include "../gvals.h"
#include "../grocery/string_ex.h"

#include <WinSock2.h>
#include <iphlpapi.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

#pragma comment(lib, "Iphlpapi.lib")

_GURU_BEGIN

/*
** NIC 
*/
class niceer
{
public:

	/*
	** IP address
	*/
	class IP
	{
	public:
		IP(std::string ip, std::string mask, std::string gate) noexcept
			: _ip(ip), _mask(mask), _gate(gate) {}

		std::string ip() const noexcept { return _ip; }
		std::string mask() const noexcept { return _mask; }
		std::string gate() const noexcept { return _gate; }

	private:
		const std::string _ip;
		const std::string _mask;
		const std::string _gate;
	};

	/*
	** a NIC
	*/
	class NIC
	{
	public:
		NIC(std::string name, std::string desc, uint32_t type) noexcept
			: _name(name), _desc(desc), _type(type) {}

		std::string name() const noexcept
		{
			return _name;
		}

		std::string desc() const noexcept
		{
			return _desc;
		}

		std::string type() const noexcept
		{
			switch (_type)
			{
			case MIB_IF_TYPE_OTHER:
				return "OTHER";
			case MIB_IF_TYPE_ETHERNET:
				return "ETHERNET";
			case MIB_IF_TYPE_TOKENRING:
				return "TOKENRING";
			case MIB_IF_TYPE_FDDI:
				return "FDDI";
			case MIB_IF_TYPE_PPP:
				return "PPP";
			case MIB_IF_TYPE_LOOPBACK:
				return "LOOPBACK";
			case MIB_IF_TYPE_SLIP:
				return "SLIP";
			case IF_TYPE_IEEE80211:
				return "IEEE802.11";
			default:
				return "UNKNOWN";
			}
		}

		NIC& append_mac(uint8_t addr) noexcept
		{
			_mac.push_back(addr);
			return *this;
		}
		NIC& append_ip(IP ip) noexcept
		{
			_ips.push_back(ip);
			return *this;
		}

		const std::string mac() const noexcept
		{
			std::ostringstream oss;
			bool append = false;
			for_each(_mac.cbegin(), _mac.cend(), [&](uint8_t v)
			{
				if (append)	oss << "-";
				append = true;
				oss << std::setw(2) << std::setfill('0') << std::hex << (int32_t)v;
			});
			return oss.str();
		}

		const std::string mask(const std::string ip) const noexcept
		{
			for_each(_ips.cbegin(), _ips.cend(), [&](const IP item)
			{
				if (ip == item.ip()) return item.mask();
			});
			return "";
		}

		std::vector<IP> ip_list() const noexcept
		{
			return _ips;
		}

	private:
		const std::string _name;
		const std::string _desc;
		const uint32_t _type;
		std::vector<uint8_t> _mac;
		std::vector<IP> _ips;
	};

	niceer() noexcept
	{
		PIP_ADAPTER_INFO info = new IP_ADAPTER_INFO();
		auto size = sizeof(IP_ADAPTER_INFO);
		auto ret = GetAdaptersInfo(info, (PULONG)&size);

		if (ERROR_BUFFER_OVERFLOW == ret)
		{
			delete info;
			info = (PIP_ADAPTER_INFO)new uint8_t[size];
			ret = GetAdaptersInfo(info, (PULONG)&size);
		}
		if (ERROR_SUCCESS == ret)
		{
			while (nullptr != info)
			{
				NIC nic(info->AdapterName, info->Description, info->Type);
				for (uint32_t i = 0; i < info->AddressLength; ++i)
					nic.append_mac(info->Address[i]);

				IP_ADDR_STRING * addr = &(info->IpAddressList);
				do
				{
					IP ip(addr->IpAddress.String, addr->IpMask.String, info->GatewayList.IpAddress.String);
					nic.append_ip(ip);
					addr = addr->Next;
				} while (nullptr != addr);

				_nics.push_back(nic);
				info = info->Next;
			}
		}

		if (info != nullptr) delete info;
	}

	std::vector<NIC> nic_list() const noexcept
	{
		return _nics;
	}

	std::string broad_ip(std::string const ip) const noexcept
	{
		std::string ret = "255.255.255.255";
		for_each(_nics.begin(), _nics.end(),
			[&](niceer::NIC nic)
		{
			auto ips = nic.ip_list();
			for_each(ips.begin(), ips.end(),
				[&](niceer::IP item)
			{
				if (item.ip() == ip)
					ret = _calc_broad(item.ip(), item.mask());
			});
		});
		return ret;
	}

private:
	std::vector<NIC> _nics;

private:
	std::string _calc_broad(std::string const ip, std::string const mask) const noexcept
	{
		std::vector<std::string> s_ip = split(ip, '.');
		std::vector<std::string> s_mask = split(mask, '.');

		if ((s_ip.size() != 4) || (s_mask.size() != 4))
			return "255.255.255.255";

		std::ostringstream oss;
		for (int i = 0; i < 4; ++i)
		{
			if (i != 0) oss << ".";
			if (std::string("255") == s_mask[i])
				oss << s_ip[i];
			else
				oss << "255";
		}
		return oss.str();
	}
};

_GURU_END

#endif /* _GURU_NICEER_H_ */