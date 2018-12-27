
#ifndef __RPC_RPCSERVICE_H__
#define __RPC_RPCSERVICE_H__

#include <string>
#include "NetEvent.h"
#include "EndPoint.h"
#include "Transport.h"
#include "Protocol.h"

namespace rpc {
template<typename E,typename T,typename P>
class RpcService{

public:
    RpcService(NetEvent* event,EndPoint ep):event_(event),ep_(ep),listener_(nullptr){ }

    ~RpcService(){ 
        close();
        event_=nullptr;
    } 

    int open(){
        listener_=connListener(ep_,this);
        if listener_==nullptr{
            return -1;
        }
        return 0;
    } 

    struct evconnlistener* connListener(const EndPoint& ep , void*userData){

        auto base=event->getInstance()->getEventBase();

        auto listener= evconnlistener_new_bind(base,
                                          listener_cb,
                                          (void *)userData,
                                          LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
                                          (struct sockaddr *)ep.getAddrIn(),
                                          sizeof(struct sockaddr));

        return listener;
    } 


    int close(){
        if (listener_!=nullptr){
            evconnlistener_free(listener);
            listener_=nullptr;
        }
    }


private:
    static int  makeServiceHandler(EventHandler*& eh,struct bufferevent * be) {
        eh=new E();
        eh->init(new T(),new P(),be);
    }

    static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *sa, int socklen, void *userData){

        struct event_base base*=event->getInstance()->getEventBase();

        struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if (nullptr!=bev) {
            event_->endEventLoop();
            return;
        }

        EventHandler* handler;
        RpcService::makeServiceHandler(handler,bev);

        bufferevent_setcb(bev, EventHandler::conn_readcb, EventHandler::conn_writecb, EventHandler::conn_eventcb, handler);
        bufferevent_enable(bev, EV_WRITE|EV_READ);
    }


    NetEvent*   event_;
    EndPoint    ep_;

	struct evconnlistener *listener_;
};
}
#endif