#include "BaseUtil.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
namespace rr
{
void GetFirstMacAddress(std::vector<std::string> &macs)
{
	printf("Enter GetFirstMacAddress\n");
	try
	{
		struct ifreq ifr;
		struct ifconf ifc;
		char buf[2048];
		int success = 0;

		int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (sock == -1)
		{
			printf("socket error\n");
			return;
		}

		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = buf;
		if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
		{
			printf("ioctl error\n");
			return;
		}

		struct ifreq* it = ifc.ifc_req;
		const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));
		char szMac[64];
		int count = 0;
		for (; it != end; ++it)
		{
			strcpy((char*)ifr.ifr_name, (const char*)it->ifr_name);
			if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0)
			{
				if (!(ifr.ifr_flags & IFF_LOOPBACK))
				{
					if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0)
					{
						count++;
						unsigned char * ptr;
						ptr = (unsigned char  *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
						snprintf(szMac, 64, "%02X:%02X:%02X:%02X:%02X:%02X", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
						std::string item(szMac);
						macs.push_back(item);
					}
				}
			}
			else
			{
				printf("get mac info error\n");
				return;
			}
		}
	}
	catch (std::exception& e)
	{
		printf("get mac info exception exception code=%s\n", e.what());
	}
	catch (...)
	{
		printf("unknown exception\n");
	}
	printf("Normal Return GetFirstMacAddress\n");
	return;
}

void GetLocalIP(std::string& ip)
{
	printf("Enter GetLocalIP\n");
	try
	{
		int sock_get_ip;
		char ipaddr[50];

		struct   sockaddr_in *sin;
		struct   ifreq ifr_ip;

		if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			printf("socket create failse...GetLocalIp!");
			return;
		}

		memset(&ifr_ip, 0, sizeof(ifr_ip));
		strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);
		if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0)
		{
			printf("can not find ip!\n");
			return;
		}
		sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
		strcpy(ipaddr, inet_ntoa(sin->sin_addr));
		close(sock_get_ip);
		ip.assign(ipaddr);
	}
	catch (std::exception& e)
	{
		printf("get mac info exception exception code=%s\n", e.what());
	}
	catch (...)
	{
		printf("unknown exception\n");
	}
	printf("Normal Return GetLocalIP\n");
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

}


