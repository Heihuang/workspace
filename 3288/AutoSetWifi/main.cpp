#include <stdio.h>
#include "src/factory/mqfactory.h"
#include "src/message/recvmessagemq.h"
#include "src/servicemodules/AutoSetWifi.h"

int main(int argc, char** argv)
{
	printf("--	\v_\v_	--\n");
	rr::MqProcessor mq;
	rr::RecvRtmessageMQ* rtmq = new rr::RecvRtmessageMQ();
	if (rtmq == NULL)
	{
		printf("rtmq is NULL\n");
		return 0;
	}
	rr::AutoSetWifi* wifi = new rr::AutoSetWifi();
	if (wifi == NULL)
	{
		printf("wifi is NULL \n");
		return 0;
	}
	rtmq->setDevUpdate(wifi);
	rtmq->RegisterMQ();
	rtmq->Start();
	while (true)
	{
		sleep(5);
	}
	delete rtmq;
	rtmq = NULL;
	delete wifi;
	wifi = NULL;
	return 0;
}

