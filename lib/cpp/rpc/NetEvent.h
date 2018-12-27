
#ifndef __RPC_NETEVENT_H__
#define __RPC_NETEVENT_H__
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}
#include "RpcChannel.h"
#include "RpcService.h"
#include "EndPoint.h"

namespace rpc {
class NetEvent {
 public:
    NetEvent(){
    #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(0x0201, &wsaData);
    #endif
    	base_ = event_base_new();
    }

    ~NetEvent(){
        if (nullptr!=base_) {
    	    event_base_free(base_);
        }
    }

    static NetEvent* getInstance(){}

    struct event_base* getEventBase(){
        return base_;
    }

    int eventLoop(){
	    event_base_dispatch(base_);
    }

    int endEventLoop(){
        event_base_loopbreak(base_);
    }

private:

	struct event_base*  base_;
};
}
#endif 