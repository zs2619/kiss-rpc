
#ifndef __RPC_RPCChANNEL_H__
#define __RPC_RPCCHANNEL_H__

#include <string>
#include "rpc/EndPoint.h"
#include "rpc/Connection.h"
#include "rpc/NetEvent.h"
#include "rpc/EventHandler.h"
#include "rpc/RpcMessage.h"


namespace rpc {
template<typename T,typename P>
class RpcChannel:public Connection{
public:

    RpcChannel(NetEvent*  event,EndPoint ep):event_(event),ep_(ep){
		struct event_base* base = event_->getInstance()->getEventBase();
		bev_ = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
		RpcAssert (bev_ != nullptr);
        int  ret= connect();
		RpcAssert (-1 != ret);
    }
	
    ~RpcChannel(){
        event_=nullptr;
        bufferevent_free(bev_);
    }
    template < typename E >
    E* createStub(){
        if (stubMap_.find(E::getObjName)!=stubMap_.end()) {
            return (E*)stubMap_[E::getObjName];
        }
		E* eh=new E();
        eh->init(std::make_shared<T>(),std::make_shared<P>());
        stubMap_[E::getObjName]=eh;
        return eh;
    }

private:

    static void connect_cb(struct bufferevent *bev, short events, void *ctx){

        RpcChannel* handler=(RpcChannel*)ctx;
        if (events & BEV_EVENT_EOF) {
        }else if (events & BEV_EVENT_ERROR) {
        }else if (events & BEV_EVENT_CONNECTED) {
            handler->setHandler();
        }else if (events&BEV_EVENT_TIMEOUT)
        {
        }
    }

    int connect(){

        bufferevent_setcb(bev_, NULL , NULL , connect_cb, this);
        bufferevent_enable(bev_, EV_WRITE);

        if (0!=bufferevent_socket_connect(bev_,(const sockaddr*)ep_.getAddrIn(),sizeof(struct sockaddr_in))){
            return -1;
        }
        return 0;
    }
    EndPoint                      ep_;
    NetEvent*                     event_; 
    struct bufferevent*           bev_;
    std::map<const char* ,ClientStub*> stubMap_;
};
}
#endif