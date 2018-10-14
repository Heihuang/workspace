#ifndef USB_CAMARAREGIST_H_
#define USB_CAMARAREGIST_H_
#include <string>
#include "../message/sendmessageMQ.h"

namespace rr
{
class UsbCamaraRegist
{
public:
	UsbCamaraRegist();
	~UsbCamaraRegist();
	void Regist();
	void setDevId(const std::string& devid);
	void setIPCId(const std::string& devid);
private:
	
	std::string ipc_id_;
	std::string dev_id_;
	SendUsbCamaraRegistMessageMQ *sender_;
};
}
#endif
