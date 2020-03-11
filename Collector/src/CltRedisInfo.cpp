/*
 * CltRedisInfo.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: zhou
 */
#include "CltRedisInfo.h"
#include <string>
namespace lxh{
CltRedisInfo::CltRedisInfo(){

}

CltRedisInfo::~CltRedisInfo(){

}

void CltRedisInfo::Collect(){
	if(sender_)
	{
		std::string test("test");
		sender_->SendMsg(test);
	}
}

}


