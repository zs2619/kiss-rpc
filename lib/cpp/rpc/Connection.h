
#ifndef __RPC_CONNECTION_H__
#define __RPC_CONNECTION_H__
#include "rpc/NetEvent.h"
namespace rpc {

class Connection
{
public:
    Connection():isValid_(false),bev_(nullptr){}
    ~Connection(){
        bev_=nullptr;
        isValid_=false;
    }
    void setBufferEvent(struct bufferevent *bev){
        bev_=bev;
    }
	struct bufferevent * getBufferEvent() {
		return bev_ ;
	}
	int setHandler() {
        bufferevent_setcb(bev_, Connection::conn_readcb, Connection::conn_writecb, Connection::conn_eventcb, this);
        evutil_socket_t fd = bufferevent_getfd(bev_);
        int one = 1;
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof one);
        return bufferevent_enable(bev_, EV_WRITE|EV_READ);
	}
 protected:
    static void conn_eventcb(struct bufferevent *bev, short events, void *userData);

    static void conn_readcb(struct bufferevent *bev, void *userData);

    static void conn_writecb(struct bufferevent *bev, void *userData);



protected:
	bool                          isValid_;
    struct bufferevent*           bev_;
};
    
    
}
#endif