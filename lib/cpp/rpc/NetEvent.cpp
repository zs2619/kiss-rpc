
#include "NetEvent.h"

rpc::NetEvent* rpc::NetEvent::event_=nullptr;

inline rpc::NetEvent::NetEvent() {
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(0x0201, &wsaData);
#endif
	base_ = event_base_new();
}

rpc::NetEvent::~NetEvent() {
	if (nullptr != base_) {
		event_base_free(base_);
	}
}

 rpc::NetEvent * rpc::NetEvent::getInstance() {
	if (event_ == nullptr) {
		event_ = new NetEvent();
	}
	return event_;
}


 int rpc::NetEvent::cancelTimer(int id) { return -1; }

 int rpc::NetEvent::eventLoop() {
	return  event_base_dispatch(base_);
}

int rpc::NetEvent::endEventLoop() {
	return event_base_loopbreak(base_);
}
