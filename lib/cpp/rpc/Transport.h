
#ifndef	__RPC_TRANSPORT_H__
#define	__RPC_TRANSPORT_H__
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}
#include "rpc/RpcMessage.h"
#include "rpc/NetEvent.h"
namespace rpc {
class Connection;
class ITransport {
public:
	ITransport(Connection* conn): bev_(nullptr),conn_(conn){
	}

	virtual ~ITransport(){
		if (bev_!=nullptr){
			bufferevent_free(bev_);
		}
	}

	virtual int sendRequestMsg(const RequestMsg& reqMsg) = 0;
	virtual int recvResponseMsg(struct evbuffer* buff, std::vector<ResponseMsg>& msg) = 0;
	virtual int sendResponseMsg(const ResponseMsg& respMsg) = 0;
	virtual int recvRequestMsg(struct evbuffer* buff, std::vector<RequestMsg>& msgVec) = 0;

	int setBufferEvent(struct bufferevent* bev) {
		if (bev_!=nullptr){
			bufferevent_free(bev_);
		}
		bev_=bev;

		bufferevent_setcb(bev_, ITransport::conn_readcb, ITransport::conn_writecb, ITransport::conn_eventcb, this);
		evutil_socket_t fd = bufferevent_getfd(bev_);
		int one = 1;
		setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&one, sizeof one);
		int ret=bufferevent_enable(bev_, EV_WRITE|EV_READ);
			return ret;
		return 0;
	}

	Connection* conn_;
protected:
	static void conn_eventcb(struct bufferevent *bev, short events, void *userData);
	static void conn_readcb(struct bufferevent *bev, void *userData);
	static void conn_writecb(struct bufferevent *bev, void *userData);
protected:
	bufferevent*  bev_;
};

class TcpTransport :public ITransport {
public:
	TcpTransport(Connection* conn):ITransport(conn){}
	virtual ~TcpTransport(){}

	virtual int sendRequestMsg(const RequestMsg& reqMsg);
	virtual int recvResponseMsg(struct evbuffer* buff, std::vector<ResponseMsg>& msg) ;

	virtual int sendResponseMsg(const ResponseMsg& respMsg);
	virtual int recvRequestMsg(struct evbuffer* buff, std::vector<RequestMsg>& msgVec);

	std::vector<byte> recvbuff_;
	std::vector<byte> sendbuff_;
};

class HttpTransport :public ITransport {
};
}
#endif