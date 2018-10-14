// FaceCapture.cpp : 定义控制台应用程序的入口点。
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include "src/servicemodules/Camera.h"
#include "src/factory/Mqfactory.h"
#include "src/message/Recvmessagemq.h"
#include "src/baseutils/BaseUtil.h"
#include "src/servicemodules/ManageCamera.h"
#include "src/message/SendmessageMQ.h"
#include "src/baseutils/RrConfig.h"
#include "logprocessor.h"
#include "src/servicemodules/ManageDev.h"

static volatile int close_sig = 1;

void SigHandle(int sig)
{
	switch(sig)
	{
	case SIGINT:
		close_sig = sig;
		break;
	default:
		break;
	}
	exit(1);
}

int main(int argc, char** argv)
{
	//signal(SIGINT, SigHandle);
	static int start = 0;
	while(start < 10)
	{
		printf("wait start...\n");
		sleep(1);
		++start;
	}
	rr::MqProcessor mq;
	LogProcessor lp;
	std::vector<std::string> macs;
	rr::GetFirstMacAddress(macs);
	if (macs.empty())
	{
		printf("find not mac\n");
		return -1;
	}
	rr::ManageDev mgdev;
	rr::RrConfig config;
	config.ReadConfig("config.ini");
	int crtype = config.ReadInt("CamreaSelect", "Enable", 0);
	std::string devid;
	devid = config.ReadString("MYSQL", "DevID", "001");
	if (devid.empty())
	{
		devid = mgdev.QueryTheDevID();
	}
	printf("get devid=%s", devid.c_str());
	rr::ManageCamera *mgcr = new rr::ManageCamera();
	if (mgcr == NULL)
	{
		printf("mgcr NULL\n");
		return -1;
	}
	mgcr->setDevId(devid);
	mgcr->setCameraType(crtype);
	rr::IpcCameraNotifyMQ* ipcrecv = NULL;
	rr::SendGetIpcId* sender = NULL;
	if (crtype == 1)
	{
		ipcrecv = new rr::IpcCameraNotifyMQ();
		if (ipcrecv == NULL)
		{
			printf("ipcrecv NULL\n");
			return -1;
		}
		ipcrecv->setDevId(devid); 
		ipcrecv->setManageCamera(mgcr);
		ipcrecv->RegisterMQ();
		ipcrecv->Start();
		sender = new rr::SendGetIpcId();
		if (sender == NULL)
		{
			return -1;
		}
		sender->setMac(devid);
		sender->SendMsg();
	}
	else
	{
		mgcr->OpenUsbCamera("1");
	}
	rr::FullViewNotifyMQ *fvnmq = new rr::FullViewNotifyMQ();
	if (fvnmq == NULL)
	{
		printf("fvnmq NULL\n");
		return -1;
	}
	fvnmq->setDevId(devid);
	fvnmq->setManageCamera(mgcr);
	fvnmq->RegisterMQ();
	fvnmq->Start();
#if 1
	while (true)
	{
		if (!mgcr->QuitMainThread())
		{
			break;
		}
		sleep(5);
	}
#endif
	delete mgcr;
	mgcr = NULL;
	fvnmq->Stop();
	delete fvnmq;
	fvnmq = NULL;
	if (ipcrecv != NULL)
	{
		ipcrecv->Stop();
		delete ipcrecv;
		ipcrecv = NULL;
	}
	if (sender != NULL)
	{
		delete sender;
		sender = NULL;
	}
	printf("closed!!!bye!!\n");
	return 0;
}

