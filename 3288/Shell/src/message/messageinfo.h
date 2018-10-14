#ifndef MESSAGEINFO_H
#define MESSAGEINFO_H
#include <string>
#include <vector>
namespace rr
{

class MessageInfo
{
public:
	MessageInfo();
	virtual ~MessageInfo();
private:
	std::string dev_id_;
	std::string dev_type_;
	std::string dev_model_;
	std::string dev_ip_;
	std::string IPC_id_;
public:
	void setDevId(const std::string &dev_id)
	{
		dev_id_ = dev_id;
	}
	std::string getDevId()const
	{
		return dev_id_;
	}
	void setDevType(const std::string& dev_type)
	{
		dev_type_ = dev_type;
	}
	std::string getDevType()const
	{
		return dev_type_;
	}
	void setDevModel(const std::string& dev_model)
	{
		dev_model_ = dev_model;
	}
	std::string getDevModel()const
	{
		return dev_model_;
	}
	void setDevIp(const std::string& dev_ip)
	{
		dev_ip_ = dev_ip;
	}
	std::string getDevIp()const
	{
		return dev_ip_;
	}
	void setIPCId(const std::string& IPC_id)
	{
		IPC_id_ = IPC_id;
	}
	std::string getIPCId()const
	{
		return IPC_id_;
	}
};

}

#endif // MESSAGEINFO_H
