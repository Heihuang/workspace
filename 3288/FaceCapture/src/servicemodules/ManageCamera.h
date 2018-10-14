#ifndef MANAGECAMERA_H_
#define MANAGECAMERA_H_

#include <map>
#include <string>
#include "Camera.h"
#include "IPCamera.h"
#include "UsbCamera.h"
#include "MysqlDao.h"
#include "ManageDB.h"
#include <mutex>

namespace rr
{
typedef struct _IPCMODE
{
	std::string ipc_id_;
	std::string ipc_no_;
	std::string ipc_url_;
	std::string ipc_ip_;
	int ipc_port_;
	Camera* camera_;
	bool addipc_flag_;

}IPCMODE;



class ManageCamera : public ManageDB
{
public:
	ManageCamera();
	~ManageCamera();
	void DelIpc(const std::string& ipcid);
	void UpdataIpc(const std::string& ipcid, const std::string& ipc_url, const std::string& ipc_ip, const int& ipc_port, const std::string&ipc_No);
	void setDevId(const std::string& devid);
	std::string getDevId()const;
	void setCameraType(const int& type);
	Camera* getCamera(const std::string& key);
	void OpenUsbCamera(const std::string& no);
	void OpenIPC();
	bool QuitMainThread();
private:
	void CreateIpcMode(IPCMODE& mode, const std::string& ipcid, const std::string& ipc_url, const std::string& ipc_ip, const int& ipc_port, const std::string&ipc_No);
	void ClearMode(IPCMODE& mode);
	void CheckCamera(const std::string& key);
	
private:
	bool thread_flag_;
	std::map<std::string, IPCMODE> ipcamera_;
	std::map<std::string, Camera*> usbcamera_;
	std::string devId_;
	std::mutex ipc_lock_;
	int camera_type_;
	Camera* camera_;
	int ipc_num_;
};

}

#endif

