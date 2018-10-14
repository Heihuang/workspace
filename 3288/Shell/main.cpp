// Shell.cpp : 定义控制台应用程序的入口点。
//
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include "src/servicemodules/DeviceRegist.h"
#include "src/servicemodules/DeviceStatusUpdate.h"
#include "src/servicemodules/UsbCamaraRegist.h"
#include "src/servicemodules/UsbUpdate.h"
#include "src/factory/mqfactory.h"
#include "src/BaseUtil.h"
#include "src/RrConfig.h"
#include "src/message/recvmessagemq.h"

void InitDevice(std::vector<std::string> &mac, std::string &ip)
{
	rr::GetFirstMacAddress(mac);
	rr::GetLocalIP(ip);
}

int main(int argc, char** argv)
{
	static int start = 0;
	while (start < 10)
	{
		printf("wait start...\n");
		++start;
		sleep(1);
	}
	rr::MqProcessor mq;
	//get mac
	std::vector<std::string> mac;
	std::string ip;
	std::string devid;
	std::string ipcid;
	InitDevice(mac, ip);
	if (mac.empty())
	{
		getchar();
		return 0;
	}
	printf("ip = %s\n", ip.c_str());
	devid = mac.front();
	ipcid = devid + "_1";
	printf("devid=%s\n", devid.c_str());
	printf("ipcid=%s\n",ipcid.c_str());
	
	rr::DeviceRegist* devreg = new rr::DeviceRegist();
	if (devreg == NULL)
	{
		getchar();
		return 0;
	}
	devreg->setDevId(devid);
	devreg->setIP(ip);
	devreg->setIPCId(ipcid);
	devreg->Regist();	
	rr::DeviceStatusUpdate *devupate = new rr::DeviceStatusUpdate();
	if (devupate == NULL)
	{
		getchar();
		return 0;
	}
	devupate->setDevId(devid);
	rr::RecvRtmessageMQ* rtmq = new rr::RecvRtmessageMQ();
	if (rtmq == NULL)
	{
		return 0;
	}
	rtmq->setDevUpdate(devupate);
	rtmq->RegisterMQ();
	rtmq->Start();
	rr::UsbUpdate *usb = new rr::UsbUpdate();
	if (usb == NULL)
	{
		printf("usb is null\n");
		return 0;
	}
	usb->setIPCId(ipcid);
	usb->Start();
	while (true)
	{
		sleep(5);
	}
	delete devreg;
	devreg = NULL;
	delete devupate;
	devupate = NULL;
	rtmq->Stop();
	delete rtmq;
	rtmq = NULL;
	usb->Stop();
	delete usb;
	usb = NULL;
	return 0;
}

