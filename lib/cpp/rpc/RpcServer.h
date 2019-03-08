
#ifndef __RPC_RPCSERVER_H__
#define __RPC_RPCSERVER_H__

#include "rpc/Common.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"
#include "rpc/ServiceProxy.h"
#include "rpc/RpcService.h"

namespace rpc {
template<typename T,typename P>
class RpcServer{
public:
    RpcServer(NetEvent* event,EndPoint ep):event_(event),ep_(ep),listener_(nullptr){

    }
    ~RpcServer(){
        close();
        event_=nullptr;
    }
    struct evconnlistener* connListener(const EndPoint& ep , void*userData){

        auto base=event_->getInstance()->getEventBase();

        return evconnlistener_new_bind(base,
                                          listener_cb,
                                          (void *)userData,
                                          LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                                          (struct sockaddr *)ep.getAddrIn(),
                                          sizeof(struct sockaddr));

    } 

    int open(){
        listener_=connListener(ep_,this);
        if (listener_==nullptr){
            return -1;
        }
        return 0;
    } 
    int close(){
        if (listener_!=nullptr){
            evconnlistener_free(listener_);
            listener_=nullptr;
}
		return 0;
    }
    template < typename E >
    bool addService(){
        proxyMap_[E::getObjName]=E();
    }
private:

    static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *sa, int socklen, void *userData){

		RpcServer* server = (RpcServer*)userData;
        struct event_base *base=server->event_->getInstance()->getEventBase();

        struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if (nullptr==bev) {
            server->event_->endEventLoop();
            return;
        }

        RpcService<T,P>::makeServiceHandler(bev,server->proxyMap_);
    }

    NetEvent*   event_;
    EndPoint    ep_;

    std::map<const char* ,ServiceProxy> proxyMap_;
	struct evconnlistener *listener_;
};
}
#endif