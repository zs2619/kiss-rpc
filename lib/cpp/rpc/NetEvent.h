
#ifndef __RPC_NETEVENT_H__
#define __RPC_NETEVENT_H__

extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
}
#include "EndPoint.h"

namespace rpc {
class NetEvent {

public:

    ~NetEvent(){
        if (nullptr!=base_) {
    	    event_base_free(base_);
        }
    }

	static NetEvent* getInstance() { 
        if (event_==nullptr){
            event_=new NetEvent();
        }
        return event_;
    }

    struct event_base* getEventBase(){
        return base_;
    }

    int eventLoop(){
	   return  event_base_dispatch(base_);
    }

    int endEventLoop(){
        return event_base_loopbreak(base_);
    }

private:
	static NetEvent*	event_;
	struct event_base*  base_;

private:
    NetEvent(){
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(0x0201, &wsaData);
    #endif
    	base_ = event_base_new();
    }
};
}
#endif 