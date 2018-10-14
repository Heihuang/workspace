#include "SendmessageMQ.h"
#include <string>
#include <vector>
namespace rr
{
BaseSendMessageMQ::BaseSendMessageMQ() : producer_(NULL), mqfactory_(NULL)
{

}

BaseSendMessageMQ::~BaseSendMessageMQ()
{
		
}


bool BaseSendMessageMQ::SendMsg()
{
	if (producer_)
	{
		MessageInfo msg;
		CreateMsg(msg);
		producer_->SendMsg(msg);
		return true;
	}
	else
	{
		return false;
	}
}

SendFullView::SendFullView()
{
	mqfactory_ = new FullViewProducerFactory();
	if (mqfactory_ == NULL)
	{
		printf("new FacesExtracorProducerFactory failed!!!!\n");
		return;
	}
	mqfactory_->CreateProducerProc(&producer_);
	if (producer_ == NULL)
	{
		printf("CreateProducerProc failed\n");
		return;
	}
}

SendFullView::~SendFullView()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendFullView::CreateMsg(MessageInfo &msg)
{
	msg.setBlobData(blob_data_);
	msg.setIpcID(ipc_id_);
	msg.setCurTime(cur_time_);
	msg.setDevID(dev_id_);
}

void SendFullView::setData(const std::string &ipcid, const std::string& devid, const std::vector<unsigned char> &blob, std::string cur_time)
{
	ipc_id_ = ipcid;
	dev_id_ = devid;
	blob_data_ = blob;
	cur_time_ = cur_time;
}

SendRtFace::SendRtFace()
{
	mqfactory_ = new DetectorProducerFactory();
	if (mqfactory_ == NULL)
	{
		printf("new FacesExtracorProducerFactory failed!!!!\n");
		return;
	}
	mqfactory_->CreateProducerProc(&producer_);
	if (producer_ == NULL)
	{
		printf("CreateProducerProc failed\n");
		return;
	}
}

SendRtFace::~SendRtFace()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendRtFace::CreateMsg(MessageInfo& msg)
{
	msg.setBlobData(faceinfo_.face);
	msg.setFaceScore(faceinfo_.facescore);
	msg.setYaw(faceinfo_.yaw);
	msg.setPitch(faceinfo_.pitch);
	msg.setRoll(faceinfo_.roll);
	msg.setFaceId(faceinfo_.faceid);
	msg.setEyeWidth(faceinfo_.eyewidth);
	msg.setFaceWidth(faceinfo_.facewidth);
	msg.setDefinition(faceinfo_.definition);
	msg.setBrightness(faceinfo_.brightness);
	msg.setCurTime(faceinfo_.curtime);
	msg.setIpcID(faceinfo_.IPCId);
	msg.setDevID(faceinfo_.devId);
	msg.setFaceQualityScore(faceinfo_.facequalityscore);
	msg.setFaceSize(faceinfo_.facesize);
	msg.setTalkId(faceinfo_.talkid);
	msg.setFullViewData(faceinfo_.fullview);
}

void SendRtFace::setData(const FaceInfo& faceinfo)
{
	faceinfo_ = faceinfo;
}


SendGetIpcId::SendGetIpcId() : mac_("")
{
	mqfactory_ = new GetIpcIdProducerFactory();
	if (mqfactory_ == NULL)
	{
		printf("new FacesExtracorProducerFactory failed!!!!\n");
		return;
	}
	mqfactory_->CreateProducerProc(&producer_);
	if (producer_ == NULL)
	{
		printf("CreateProducerProc failed\n");
		return;
	}
}

SendGetIpcId::~SendGetIpcId()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendGetIpcId::CreateMsg(MessageInfo& msg)
{
	
	msg.setDevID(mac_);
}

void SendGetIpcId::setMac(const std::string& mac)
{
	mac_ = mac;
}

/**************************************************************/
SendFaceUpdate::SendFaceUpdate()
{
	mqfactory_ = new FaceUpdateProducerFactory();
	if (mqfactory_ == NULL)
	{
		printf("new FacesExtracorProducerFactory failed!!!!\n");
		return;
	}
	mqfactory_->CreateProducerProc(&producer_);
	if (producer_ == NULL)
	{
		printf("CreateProducerProc failed\n");
		return;
	}
}

SendFaceUpdate::~SendFaceUpdate()
{
	if (mqfactory_)
	{
		mqfactory_->DestroyProducerProc(producer_);
		delete mqfactory_;
		mqfactory_ = NULL;
	}
}

void SendFaceUpdate::CreateMsg(MessageInfo& msg)
{
	msg.setDevID(devId_);
	msg.setCurTime(curtime_);
	msg.setIpcID(IPCId_);
	msg.setFaceId(faceid_);
}

void SendFaceUpdate::setData(const std::string& faceid, const std::string& curtime, const std::string& ipcid, const std::string& devid)
{
	faceid_ = faceid;
	curtime_ = curtime;
	IPCId_ = ipcid;
	devId_ = devid;
}
}
