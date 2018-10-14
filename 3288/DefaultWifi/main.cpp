#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "servicemodules/managdb.h"
#include "baseutils/BaseUtil.h"

int main(int argc, char** argv)
{
	Managdb db;
	std::string isActived = db.QueryIsActivated();
	int autowifi = db.QueryIsAutoWifi();
	if (isActived == "true")
	{
		printf("Active is succeed!");
		return 0;
	}
	if (autowifi == 0)
	{
		system("sudo cp FaceOS_Wifi /etc/NetworkManager/system-connections");
		db.UpdateAutoWifi();
		system("sudo reboot");
		return 0;
	}
	std::string config = "./FaceOS_Wifi";
	std::string mac = "00:00:00:00:00:00";
	std::vector<std::string> macs;
	rr::GetFirstMacAddress(macs);
	if (!macs.empty())
	{
		mac = macs[0];
	}
	std::string id = "FaceOS_" + mac;
	std::string ssid = id;
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(config.c_str(), pt);
	pt.put<std::string>("connection.id", id);
	pt.put<std::string>("wifi.ssid", ssid);
	boost::property_tree::ini_parser::write_ini(config.c_str(), pt);
	std::string cmd = "sudo cp FaceOS_Wifi /etc/NetworkManager/system-connections/FaceOS_";
	cmd += mac;
	system(cmd.c_str());
	db.UpdateActivate();
	system("sudo rm /etc/NetworkManager/system-connections/FaceOS_Wifi");
	system("sudo reboot");
	return 0;
}

