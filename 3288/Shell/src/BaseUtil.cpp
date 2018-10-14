#include "BaseUtil.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>  
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
namespace rr
{
void GetFirstMacAddress(std::vector<std::string> &macs)
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

void GetLocalIP(std::string& ip)
{
#if 0
	int sock_get_ip;
	char ipaddr[50];

	struct   sockaddr_in *sin;
	struct   ifreq ifr_ip;

	if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket create failse...GetLocalIp!/n");
		return;
	}
	if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0)
	{
		printf("get ip failed\n");
		return;
	}
	memset(&ifr_ip, 0, sizeof(ifr_ip));
	strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);
	sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
	strcpy(ipaddr, inet_ntoa(sin->sin_addr));
	ip.assign(ipaddr);
	if (ip.empty())
	{
		printf("wifi\n");
		memset(&ifr_ip, 0, sizeof(ifr_ip));
		strncpy(ifr_ip.ifr_name, "wlan0", sizeof(ifr_ip.ifr_name) - 1);
		sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;
		strcpy(ipaddr, inet_ntoa(sin->sin_addr));
		ip.assign(ipaddr);
	}
	else
	{
		printf("ip Error \n");
	}
	close(sock_get_ip);
#endif
	struct ifaddrs * ifAddrStruct = NULL;
	void * tmpAddrPtr = NULL;
	std::string ethip;
	std::string wlanip;
	getifaddrs(&ifAddrStruct);
	ip = "127.0.0.1";
	while (ifAddrStruct != NULL)
	{
		if (ifAddrStruct->ifa_addr->sa_family == AF_INET)
		{ // check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			printf("%s IP Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
			if (strcmp((const char*)ifAddrStruct->ifa_name, "eth0") == 0)
			{
				ethip.assign((const char*)addressBuffer);
			}
			if (strcmp((const char*)ifAddrStruct->ifa_name, "wlan0") == 0)
			{
				wlanip.assign((const char*)addressBuffer);
			}
		}
		ifAddrStruct = ifAddrStruct->ifa_next;
	}
	if (!ethip.empty())
	{
		ip = ethip;
	}
	if (!wlanip.empty())
	{
		ip = wlanip;
	}
	
	
}

}


