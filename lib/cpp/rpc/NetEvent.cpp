
#include "rpc/NetEvent.h"

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



int rpc::NetEvent::eventLoop() {
	return  event_base_dispatch(base_);
}

int rpc::NetEvent::endEventLoop() {
	return event_base_loopbreak(base_);
}
event* rpc::NetEvent::scheduleTimer(event_callback_fn cb,void* arg, const timeval& delay, const timeval & interval ){
	event* ev=  event_new(base_, -1, EV_PERSIST, cb, arg);
	if (ev == nullptr){
		return  nullptr;
	}
	if (-1 == event_add(ev, &interval)) {
		event_del(ev);
		return  nullptr;
	}
	return ev;
}

void rpc::NetEvent::cancelTimer(event * ev) { event_free(ev); }
