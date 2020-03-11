/*
 * Sender.cpp
 *
 *  Created on: Jun 26, 2019
 *      Author: zhou
 */
#include "Sender.h"

namespace lxh{
Sender::Sender():net_(nullptr), flg_(false), t_id_(0){
	msgs_.reserve(Q_MAX_SIZE);
	net_ = new Net();
	pthread_mutex_init(&mtx_, nullptr);
}

Sender::~Sender(){
	msgs_.clear();
	if(net_){
		delete net_;
		net_ = nullptr;
	}
	pthread_mutex_destroy(&mtx_);
}

int Sender::SendMsg(const std::string& msg){
	pthread_mutex_lock(&mtx_);
	if(msgs_.size() < Q_MAX_SIZE){
		msgs_.push_back(msg);
	}
	pthread_mutex_unlock(&mtx_);
	return 0;
}

void Sender::Start(){
	InitNet();
	flg_ = true;
	pthread_create(&t_id_, nullptr, work, (void*)this);
}

void Sender::Stop(){
	flg_ = false;
	pthread_join(t_id_, nullptr);
	UnInitNet();
}

void* Sender::work(void* arg){
	Sender* sd = static_cast<Sender*>(arg);
	if (sd == nullptr){
		printf("create thread failed\n");
		return nullptr;
	}
	sd->RealSend();
	return nullptr;
}

void Sender::RealSend(){
	while(flg_){
		pthread_mutex_lock(&mtx_);
		if(!msgs_.empty()){
			std::string tp = msgs_.front();
			msgs_.erase(msgs_.begin());
			size_t l = net_->Write((char*)tp.c_str(), tp.length());
			printf("send msg = %s len = %d write = %d\n", tp.c_str(), tp.length(), l);
			pthread_mutex_unlock(&mtx_);
		}
		else{
			pthread_mutex_unlock(&mtx_);
		}

		sleep(1);
	}
}

int Sender::InitNet(){
	if(net_ == nullptr){
		return -1;
	}
	net_->InitConn("127.0.0.1", 10020);
	return net_->Connect();
}

int Sender::UnInitNet(){
	if(net_ == nullptr){
		return -1;
	}
	net_->CloseConn();
	return 0;
}

}


