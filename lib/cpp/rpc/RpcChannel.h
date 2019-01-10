
#ifndef __RPC_RPCChANNEL_H__
#define __RPC_RPCCHANNEL_H__

#include <string>
#include "EndPoint.h"
#include "NetEvent.h"
#include "EventHandler.h"
#include "RpcMessage.h"


namespace rpc {
template<typename E,typename T,typename P>
class RpcChannel{
public:

    RpcChannel(NetEvent*  event):event_(event){}
	
    ~RpcChannel(){
        event_=nullptr;
    }

    int createStub(E*& eh,EndPoint ep){

		struct event_base* base = event_->getInstance()->getEventBase();
		struct bufferevent * be = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
		if (be == nullptr){
			return -1;
		}
        eh=new E();
        eh->init(new T(),new P(),be);
        int  ret= connect(eh,ep);
        return ret;
    }

private:

    static void connect_cb(struct bufferevent *bev, short events, void *ctx){

        EventHandler* handler=(EventHandler*)ctx;
        if (events & BEV_EVENT_EOF) {
			handler->handleClose();
        }else if (events & BEV_EVENT_ERROR) {
			handler->handleClose();
        }else if (events & BEV_EVENT_CONNECTED) {
            handler->setHandler();
        }else if (events&BEV_EVENT_TIMEOUT)
        {

        }
    }

    int connect(E* eh,const EndPoint& ep){

        bufferevent_setcb(eh->getBufferEvent(), NULL , NULL , connect_cb, eh);
        bufferevent_enable(eh->getBufferEvent(), EV_WRITE);

        if (0!=bufferevent_socket_connect(eh->getBufferEvent(),(const sockaddr*)ep.getAddrIn(),sizeof(struct sockaddr_in))){
            return -1;
        }
        return 0;
    }
    NetEvent*   event_; 
};
}
#endif