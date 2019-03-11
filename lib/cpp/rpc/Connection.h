
#ifndef __RPC_CONNECTION_H__
#define __RPC_CONNECTION_H__
#include "rpc/NetEvent.h"
#include "rpc/Common.h"
#include "rpc/Transport.h"
#include "rpc/Protocol.h"
namespace rpc {

class Connection
{
public:
    Connection(NetEvent*  event,EndPoint ep ,struct bufferevent* bev,ITransport* trans,IProtocol* proto):
    event_(event),
    ep_(ep),
    isValid_(false),
    bev_(bev),
    trans_(trans),
    proto_(proto){
        if (bev_==nullptr){
            struct event_base* base = event_->getInstance()->getEventBase();
            bev_ = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
            RpcAssert (bev_ != nullptr);
        }
    }
    ~Connection(){
        bev_=nullptr;
        isValid_=false;
        trans_=nullptr;
        proto_=nullptr;

        bufferevent_free(bev_);
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

        getTransport()->setBufferEvent(bev_);
        int ret=bufferevent_enable(bev_, EV_WRITE|EV_READ);
        return ret;
	}
    virtual int handleInput(struct evbuffer* buff)=0;
    virtual int handleOutput()=0;
    virtual int handleClose()=0;
    virtual int handleConnction()=0;

    IProtocol* getProtocol() const {return proto_;}
    ITransport* getTransport()const {return trans_;}

private:
    ITransport*   trans_;
    IProtocol*    proto_;

protected:
    static void conn_eventcb(struct bufferevent *bev, short events, void *userData);

    static void conn_readcb(struct bufferevent *bev, void *userData);

    static void conn_writecb(struct bufferevent *bev, void *userData);



protected:
	bool                          isValid_;
    struct bufferevent*           bev_;
    NetEvent*                     event_; 
    EndPoint                      ep_;
};
    
    
}
#endif