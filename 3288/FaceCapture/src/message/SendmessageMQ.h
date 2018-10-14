#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include <vector>
#include <map>
#include "../factory/Mqfactory.h"
#include "Messageinfo.h"
#include "../baseutils/FaceInfo.h"

namespace rr
{
class BaseSendMessageMQ
{
public:
	BaseSendMessageMQ();
	virtual ~BaseSendMessageMQ();
	virtual void CreateMsg(MessageInfo& msg) = 0;
	bool SendMsg();
protected:
	Producer* producer_;
	MqProducerFactory *mqfactory_;
};


class SendFullView : public BaseSendMessageMQ
{
public:
	SendFullView();
	~SendFullView();
	virtual void CreateMsg(MessageInfo& msg);
	void setData(const std::string& ipcid, const std::string& devid, const std::vector<unsigned char>& blob, std::string cur_time);
private:
	std::string ipc_id_;
	std::string dev_id_;
	std::vector<unsigned char> blob_data_;
	std::string cur_time_;
};

class SendRtFace : public BaseSendMessageMQ
{
public:
	SendRtFace();
	~SendRtFace();
	virtual void CreateMsg(MessageInfo& msg);
	void setData(const FaceInfo& faceinfo);
private:
	FaceInfo faceinfo_;
};

class SendGetIpcId : public BaseSendMessageMQ
{
public:
	SendGetIpcId();
	~SendGetIpcId();
	virtual void CreateMsg(MessageInfo& msg);
	void setMac(const std::string& mac);
private:
	std::string mac_;
};

class SendFaceUpdate : public BaseSendMessageMQ
{
public:
	SendFaceUpdate();
	~SendFaceUpdate();
	virtual void CreateMsg(MessageInfo& msg);
	void setData(const std::string& faceid, const std::string& curtime, const std::string& ipcid, const std::string& devid);
private:
	std::string faceid_;
	std::string curtime_;
	std::string IPCId_;
	std::string devId_;
};

}
#endif // SENDMESSAGE_H
