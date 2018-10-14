#include "Mqfactory.h"
#include <string>
#include "../baseutils/RrConfig.h"

namespace rr
{

DetectorProducerFactory::DetectorProducerFactory()
{

}

DetectorProducerFactory::~DetectorProducerFactory()
{

}

void DetectorProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "failover:(tcp://127.0.0.1:61616)");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "SendRtFace", "ComponentTalkQueue");
		*obj = new DetectorProducer();
		if (obj != NULL)
		{
			printf("ip=%s\n", ip.c_str());
			//ip = "failover:(tcp://172.16.1.243:61616)";
			(*obj)->Connect(ip, user, password);
			(*obj)->Queue(qname);
			(*obj)->CreateRealProducer();
			printf("ip=%s connect mq sucessed!!!\n", ip.c_str());
		}
		else
		{
			printf("connect mq failed!!!!\n");
			return;
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!\n");
	}
}

void DetectorProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}


GetIpcIdProducerFactory::GetIpcIdProducerFactory()
{

}

GetIpcIdProducerFactory::~GetIpcIdProducerFactory()
{

}

void GetIpcIdProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "SendGetIPC", "GetDeviceIPCamara");
		*obj = new GetIpcIdProducer();
		if (obj != NULL)
		{
			(*obj)->Connect(ip, user, password);
			(*obj)->Queue(qname);
			(*obj)->CreateRealProducer();
			printf("ip=%s connect mq sucessed!!!\n", ip.c_str());
			//(*obj)->run();
		}
		else
		{
			printf("connect mq failed!!\n");
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!\n");
	}
}

void GetIpcIdProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

FullViewProducerFactory::FullViewProducerFactory()
{

}

FullViewProducerFactory::~FullViewProducerFactory()
{

}

void FullViewProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "failover:(tcp://127.0.0.1:61616)");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "SendVideoPreview", "VideoPreview");
		*obj = new FullViewProducer();
		if (obj != NULL)
		{
			printf("ip=%s\n", ip.c_str());
			(*obj)->Connect(ip, user, password);
			(*obj)->Queue(qname);
			(*obj)->CreateRealProducer();
			printf("connect mq succssed!!!!,qname=%s\n", qname.c_str());
		}
		else
		{
			printf("connect mq failed!!!!\n");
			return;
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!!\n");
	}
}

void FullViewProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

FullViewNotifyConsumerFactory::FullViewNotifyConsumerFactory()
{

}

FullViewNotifyConsumerFactory::~FullViewNotifyConsumerFactory()
{

}

void FullViewNotifyConsumerFactory::CreateConsumerProc(Consumer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "RecvSendVideo", "SendVideo");
		*obj = new FullViewNotifyConsumer();
		if (obj != NULL)
		{
			(*obj)->Connect(ip, user, password);
			(*obj)->Topic(qname);
			(*obj)->CreateRealConsumer();
			printf("ip=%s connect mq sucessed!!!\n", ip.c_str());
			//(*obj)->run();
		}
		else
		{
			printf("connect mq failed!!\n");
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!\n");
	}

}

void FullViewNotifyConsumerFactory::DestroyConsumerProc(Consumer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

UpdateIpcConsumerFactory::UpdateIpcConsumerFactory()
{

}

UpdateIpcConsumerFactory::~UpdateIpcConsumerFactory()
{

}

void UpdateIpcConsumerFactory::CreateConsumerProc(Consumer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "RecvUpdateIpc", "CamaraInfoUpdate");
		*obj = new UpdateIpcConsumer();
		if (obj != NULL)
		{
			(*obj)->Connect(ip, user, password);
			(*obj)->Topic(qname);
			(*obj)->CreateRealConsumer();
			printf("ip=%s connect mq sucessed!!!\n", ip.c_str());
			//(*obj)->run();
		}
		else
		{
			printf("connect mq failed!!\n");
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!\n");
	}
}

void UpdateIpcConsumerFactory::DestroyConsumerProc(Consumer *obj)
{
	if (obj)
	{
		delete obj;
		obj = NULL;
	}
}


/**********************************************************************/

FaceUpdateProducerFactory::FaceUpdateProducerFactory()
{

}

FaceUpdateProducerFactory::~FaceUpdateProducerFactory()
{

}

void FaceUpdateProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "failover:(tcp://127.0.0.1:61616)");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "SendUpdateInfo", "UpdateFaceInfo");
		*obj = new FaceUpdateProducer();
		if (obj != NULL)
		{
			printf("ip=%s\n", ip.c_str());
			(*obj)->Connect(ip, user, password);
			(*obj)->Queue(qname);
			(*obj)->CreateRealProducer();
			printf("connect mq succssed!!!!,qname=%s\n", qname.c_str());
		}
		else
		{
			printf("connect mq failed!!!!\n");
			return;
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!!\n");
	}
}

void FaceUpdateProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		delete obj;
		obj = NULL;
	}
}

}
