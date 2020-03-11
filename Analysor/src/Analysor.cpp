/*
 * Analysor.cpp
 *
 *  Created on: Jul 1, 2019
 *      Author: zhou
 */

#include "Analysor.h"
namespace lxh {

Analysor::Analysor() :
		net_(nullptr), ep_(nullptr) {

}

Analysor::~Analysor() {

}

void Analysor::Start() {
	listennet();
}

void Analysor::Close() {
	closenet();

}

void* Analysor::HandleSaveMsg(void* args) {
	Analysor* self = static_cast<Analysor*>(args);
	if (self == nullptr) {
		return nullptr;
	}
	return nullptr;
}

void Analysor::HandleCp(void* data, size_t len, void* user) {
	Analysor* self = static_cast<Analysor*>(user);
	if (self == nullptr) {
		return ;
	}
}

void Analysor::CpData(void* data, size_t len){

}

void Analysor::SaveMsg(){

}

void Analysor::listennet() {
	if (net_ == nullptr) {
		net_ = new Net();
	}
	if (ep_ == nullptr) {
		ep_ = new Epoll();
	}
	net_->InitConn("127.0.0.1", 10020);
	net_->Listen();
	ep_->setNet(net_);
	ep_->EpollInit();
	ep_->EpollDispatch();
}

void Analysor::closenet() {
	if (net_) {
		net_->CloseConn();
		delete net_;
		net_ = nullptr;
	}
	if (ep_) {
		ep_->EpollClose();
		delete ep_;
		ep_ = nullptr;
	}

}

}

