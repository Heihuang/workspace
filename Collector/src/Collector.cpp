/*
 * Collector.cpp
 *
 *  Created on: Jun 27, 2019
 *      Author: zhou
 */

#include <stdio.h>
#include "CltRedisInfo.h"

namespace lxh{

Collector::Collector():sender_(nullptr){
		sender_ = new Sender();
	}
Collector::~Collector(){
	if(sender_){
		delete sender_;
		sender_ = nullptr;
	}
}
void Collector::Ready(){
	if(sender_){
		printf("Ready\n");
		sender_->Start();
	}
}

void Collector::Down(){
	if(sender_){
		printf("Down\n");
		sender_->Stop();
	}
}

}


