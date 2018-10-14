#include "mqfactory.h"
#include <string>
#include "../RrConfig.h"
namespace rr
{

DeviceRegistProducerFactory::DeviceRegistProducerFactory()
{

}

DeviceRegistProducerFactory::~DeviceRegistProducerFactory()
{

}

void DeviceRegistProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "DeviceRegistQueuename", "DeviceRegist");
		*obj = new DeviceRegistProducer();
		if (obj != NULL)
		{
			printf("ip=%s\n", ip.c_str());
			(*obj)->Connect(ip, user, password);
			(*obj)->Queue(qname);
			(*obj)->CreateRealProducer();
			printf("connect mq succssed!!!!\n");
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

void DeviceRegistProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}


UsbCamaraRegistProducerFactory::UsbCamaraRegistProducerFactory()
{

}

UsbCamaraRegistProducerFactory::~UsbCamaraRegistProducerFactory()
{

}

void UsbCamaraRegistProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "UsbCamaraRegistQueuename", "UsbCamaraRegist");
		*obj = new UsbCamaraRegistProducer();
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

void UsbCamaraRegistProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

DeviceStatusUpdateProducerFactory::DeviceStatusUpdateProducerFactory()
{

}

DeviceStatusUpdateProducerFactory::~DeviceStatusUpdateProducerFactory()
{

}

void DeviceStatusUpdateProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "DeviceStatusUpdateQueuename", "DeviceStatusUpdate");
		*obj = new DeviceStatusUpdateProducer();
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
			printf("connect mq failed!!!!");
			return;
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!!");
	}
}

void DeviceStatusUpdateProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

UsbCamaraStatusUpdateProducerFactory::UsbCamaraStatusUpdateProducerFactory()
{

}

UsbCamaraStatusUpdateProducerFactory::~UsbCamaraStatusUpdateProducerFactory()
{

}

void UsbCamaraStatusUpdateProducerFactory::CreateProducerProc(Producer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "UsbCamaraUpdateQueuename", "UsbCameraStatusUpdate");
		*obj = new UsbCamaraStatusUpdateProducer();
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

void UsbCamaraStatusUpdateProducerFactory::DestroyProducerProc(Producer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

DeviceStatusUpdateRequestConsumerFactory::DeviceStatusUpdateRequestConsumerFactory()
{

}

DeviceStatusUpdateRequestConsumerFactory::~DeviceStatusUpdateRequestConsumerFactory()
{

}

void DeviceStatusUpdateRequestConsumerFactory::CreateConsumerProc(Consumer** obj)
{
	try
	{
		RrConfig config;
		config.ReadConfig("config.ini");
		std::string ip = config.ReadString("ACTIVEMQ", "Mqip", "127.0.0.1");
		std::string user = config.ReadString("ACTIVEMQ", "User", "admin");
		std::string password = config.ReadString("ACTIVEMQ", "Password", "admin");
		std::string qname = config.ReadString("ACTIVEMQ", "DeviceStatusUpdateRequestTopicName", "DeviceStatusUpdateRequest");
		*obj = new DeviceStatusUpdateRequestConsumer();
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
			printf("connect mq failed!!!\n");
		}
	}
	catch (...)
	{
		printf("connect mq exception failed!!!\n");
	}
}

void DeviceStatusUpdateRequestConsumerFactory::DestroyConsumerProc(Consumer* obj)
{
	if (obj)
	{
		obj->Close();
		delete obj;
		obj = NULL;
	}
}

}
