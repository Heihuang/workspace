#include "ManageCamera.h"
#include <stdio.h>
#include "IPCamera.h"
namespace rr
{

ManageCamera::ManageCamera() : camera_type_(0), devId_(""), camera_(NULL), ipc_num_(0)
{
	rr::RrConfig config;
	config.ReadConfig("config.ini");
	ipc_num_ = config.ReadInt("MYSQL", "IPCNUM", 3);
}

ManageCamera::~ManageCamera()
{

	for (std::map<std::string, IPCMODE>::iterator it = ipcamera_.begin(); it != ipcamera_.end(); ++it)
	{

		IPCMODE mode = it->second;
		ClearMode(mode);
	}
	ipcamera_.clear();
	for (std::map<std::string, Camera*>::iterator it = usbcamera_.begin(); it != usbcamera_.end(); it)
	{
		it->second->Close();
		delete it->second;

	}
	usbcamera_.clear();
}



void ManageCamera::UpdataIpc(const std::string& ipcid, const std::string& ipc_url, const std::string& ipc_ip, const int& ipc_port, const std::string&ipc_No)
{

	std::map<std::string, IPCMODE>::iterator it = ipcamera_.find(ipcid);
	IPCMODE mode;
	ipc_lock_.lock();
	if (it != ipcamera_.end())
	{

		printf("update ipc\n");
		mode = it->second;
		ClearMode(mode);
		CreateIpcMode(mode, ipcid, ipc_url, ipc_ip, ipc_port, ipc_No);
		it->second = mode;
	}
	else
	{
		printf("add new ipc\n");
		CreateIpcMode(mode, ipcid, ipc_url, ipc_ip, ipc_port, ipc_No);
		ipcamera_.insert(std::pair<std::string, IPCMODE>(ipc_No, mode));

	}
	ipc_lock_.unlock();
}



void ManageCamera::DelIpc(const std::string& ipcid)
{

	ipc_lock_.lock();
	std::map<std::string, IPCMODE>::iterator it = ipcamera_.find(ipcid);
	if (it != ipcamera_.end())
	{

		printf("del ipc\n");

		IPCMODE mode = it->second;

		ClearMode(mode);

		ipcamera_.erase(it);

	}
	ipc_lock_.unlock();
}



void ManageCamera::setDevId(const std::string& devid)
{

	devId_ = devid;

}



std::string ManageCamera::getDevId() const
{

	return devId_;

}



void ManageCamera::CreateIpcMode(IPCMODE& mode, const std::string& ipcid, const std::string& ipc_url, const std::string& ipc_ip, const int& ipc_port, const std::string&ipc_No)
{

	mode.ipc_id_ = ipcid;
	mode.ipc_url_ = ipc_url;
	mode.ipc_ip_ = ipc_ip;
	mode.ipc_port_ = ipc_port;
	mode.ipc_no_ = ipc_No;
	mode.camera_ = new IPCamera(ipc_url.c_str());
	if (mode.camera_ != NULL)
	{

		mode.camera_->setDev(devId_, ipc_ip);
		mode.camera_->Open();
	}
}



void ManageCamera::ClearMode(IPCMODE& mode)
{
	if (mode.camera_)
	{

		mode.camera_->Close();
		delete mode.camera_;
		mode.camera_ = NULL;
		printf("close camera!!!\n");
	}
}



void ManageCamera::setCameraType(const int& type)
{
	camera_type_ = type;
}



void ManageCamera::CheckCamera(const std::string& key)
{

	if (camera_type_ == 1)
	{

		std::map<std::string, IPCMODE>::iterator it = ipcamera_.find(key);
		if (it != ipcamera_.end())
		{

			camera_ = it->second.camera_;

		}
	}
	else
	{

		std::map<std::string, Camera*>::iterator it = usbcamera_.find(key);

		camera_ = it->second;
	}
}

Camera* ManageCamera::getCamera(const std::string& key)
{

	CheckCamera(key);
	return camera_;

}



void ManageCamera::OpenUsbCamera(const std::string& no)
{

	Camera *camera = new UsbCamera();
	std::map<std::string, Camera*>::iterator it = usbcamera_.find(no);
	std::string tmp = devId_ + "_" + no;
	if (it != usbcamera_.end())
	{

		it->second->setDev(devId_, tmp);

		it->second->Close();

		it->second->Open();

	}
	else
	{

		camera->setDev(devId_, tmp);

		camera->Open();

		usbcamera_.insert(std::make_pair(tmp, camera));

	}

}

void ManageCamera::OpenIPC()
{
	if (!OpenDB())
	{
		printf("open db failed\n");
		return;
	}
	int count = 0;
	IPCMODE mode;
	std::string ipcid;
	std::string ipc_url;
	std::string ipc_ip;
	int ipc_port;
	std::string ipc_No;
	std::string sql = "SELECT A.ID, A.IPC_URL, A.IPC_IP, A.IPC_PORT, A.IPC_NO FROM camarainfo AS A WHERE A.DEV_ID IN (SELECT id FROM deviceinfo where DEV_ID = '";
	sql += devId_;
	sql += " ')";
	sql += "and A.is_delete=0";
	MySqlDao::getInstance().SqlExecute(sql.c_str());
	while (MySqlDao::getInstance().Next() != NULL)
	{
		if (ipc_num_ < ++count)
		{
			break;
		}
		if (MySqlDao::getInstance().getRes(0) != NULL)
		{
			ipcid.assign(MySqlDao::getInstance().getRes(0));

		}
		if (MySqlDao::getInstance().getRes(1) != NULL)
		{
			ipc_url.assign(MySqlDao::getInstance().getRes(1));
		}
		if (MySqlDao::getInstance().getRes(2) != NULL)
		{
			ipc_ip.assign(MySqlDao::getInstance().getRes(2));
		}
		if (MySqlDao::getInstance().getRes(3) != NULL)
		{
			ipc_port = atoi(MySqlDao::getInstance().getRes(3));
		}
		if (MySqlDao::getInstance().getRes(4) != NULL)
		{
			ipc_No.assign(MySqlDao::getInstance().getRes(4));
		}
		if (!ipc_url.empty())
		{
			CreateIpcMode(mode, ipcid, ipc_url, ipc_ip, ipc_port, ipc_No);
			ipcamera_.insert(std::pair<std::string, IPCMODE>(ipc_No, mode));
			printf("open ipc=%s", ipc_No.c_str());
		}
	}
	MySqlDao::getInstance().FlushDataSet();
	CloseDB();
}

bool ManageCamera::QuitMainThread()
{
	bool ret = true;
	if (camera_type_ == 1)
	{
		for (std::map<std::string, IPCMODE>::iterator it = ipcamera_.begin(); it != ipcamera_.end(); ++it)
		{

			IPCMODE mode = it->second;
			ret = mode.camera_->getQuitState();
			if (!ret)
			{
				break;
			}
		}
	}
	else if (camera_type_ == 0)
	{
		for (std::map<std::string, Camera*>::iterator it = usbcamera_.begin(); it != usbcamera_.end(); it)
		{
			ret =  it->second->getQuitState();
			if (!ret)
			{
				break;
			}
			
		}
	}
	

	return ret;
}

}



