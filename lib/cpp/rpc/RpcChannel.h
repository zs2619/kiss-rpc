
#ifndef __RPC_RPCChANNEL_H__
#define __RPC_RPCCHANNEL_H__

#include <string>
#include "EndPoint.h"
#include "Transport.h"
#include "NetEvent.h"
#include "RpcMessage.h"
#include "Protocol.h"


namespace rpc {
template<typename E,typename T,typename P>
class RpcChannel{
public:

    RpcChannel(NetEvent*  event_):event_(event){}

    ~RpcChannel(){
        event_=nullptr;
    }

    int createStub(E*& eh,EndPoint ep){
        eh=new E();
        eh->init(new T(),new P(),be);
        int  ret= connect(eh,ep);
        return ret;
    }

private:

    static void connect_hostname_cb(struct bufferevent *bev, short what, void *ctx){
        EventHandler* handler=(EventHandler*)ctx;

        bufferevent_setcb(bev, EventHandler::conn_readcb, EventHandler::conn_writecb, EventHandler::conn_eventcb, handler);
        bufferevent_enable(bev, EV_WRITE|EV_READ);
        return 0;
    }

    int connect(E* eh,const EndPoint& ep){

        struct event_base base*=event->getInstance()->getEventBase();

	    struct bufferevent * be = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
        if be==nullptr{
            return -1;
        }

        bufferevent_setcb(be, NULL , NULL , connect_hostname_cb, eh);
        bufferevent_enable(be, EV_WRITE);

        if (0!=bufferevent_socket_connect_hostname(be,NULL, AF_INET,ep.getHostName(),ep.getPort())){
            return -1;
        }
        return 0;
    }
    NetEvent*   event_; 
};
}
#endif