
#ifndef __RPC_EVENTHANDLER_H__
#define __RPC_EVENTHANDLER_H__

#include "rpc/RpcMessage.h"
#include "rpc/NetEvent.h"
#include "rpc/Transport.h"
#include "rpc/Protocol.h"
namespace rpc {

class EventHandler{

 public:
    EventHandler():isValid_(false){}
    virtual ~EventHandler(){
        trans_=nullptr;
        proto_=nullptr;
        bev_=nullptr;
    }

    int init(std::shared_ptr<ITransport> trans,std::shared_ptr<IProtocol> proto, struct bufferevent *bev){
        trans_= trans;
        proto_= proto;
		bev_ = bev;
		trans_->setBufferEvent(bev_);
        setHandler();
        isValid_=true;
		return 0;
    }
    int fini(){
        if (bev_)
            bufferevent_free(bev_);
        delete this;
		return 0;
    }
    void setBufferEvent(struct bufferevent *bev){
        bev_=bev;
    }
	struct bufferevent * getBufferEvent() {
		return bev_ ;
	}
	int setHandler() {
        bufferevent_setcb(bev_, EventHandler::conn_readcb, EventHandler::conn_writecb, EventHandler::conn_eventcb, this);
        evutil_socket_t fd = bufferevent_getfd(bev_);
        int one = 1;
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof one);
        return bufferevent_enable(bev_, EV_WRITE|EV_READ);
	}

	virtual bool dispatch(std::shared_ptr<RpcMsg> m)=0;
    virtual int invoke(std::shared_ptr<RpcMsg> msg)=0;

    virtual int handleInput(std::vector<int8>& buff)=0;
    virtual int handleOutput()=0;
    virtual int handleClose()=0;
    virtual int handleConnction()=0;

    IProtocol* getProtocol(){return proto_.get();}
    ITransport* getTransport(){return trans_.get();}


protected:
	bool                          isValid_;
private:
    std::shared_ptr<ITransport>   trans_;
    std::shared_ptr<IProtocol>    proto_;
    struct bufferevent*           bev_;

 protected:
    static void conn_eventcb(struct bufferevent *bev, short events, void *userData);

    static void conn_readcb(struct bufferevent *bev, void *userData);

    static void conn_writecb(struct bufferevent *bev, void *userData);

};

}
#endif