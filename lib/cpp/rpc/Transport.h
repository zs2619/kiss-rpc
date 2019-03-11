
#ifndef	__RPC_TRANSPORT_H__
#define	__RPC_TRANSPORT_H__
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}
#include "rpc/RpcMessage.h"
namespace rpc {
	class ITransport {
	public:
		ITransport():connStatus_(0){}
		virtual ~ITransport(){}

		void setBufferEvent(bufferevent* bev) {
			bev_ = bev;
		}

		virtual int sendRequestMsg(struct evbuffer* buff) = 0;
		virtual int recvResponseMsg(struct evbuffer* buff, ResponseMsg& msg) = 0;

		virtual int sendResponseMsg(struct evbuffer* buff) = 0;
		virtual int recvRequestMsg(struct evbuffer* buff, RequestMsg& msg) = 0;

	protected:
		int         connStatus_;
		bufferevent* bev_;
	};

	class TcpTransport :public ITransport {
	public:
		TcpTransport(){}
		virtual ~TcpTransport(){}

		virtual int sendRequestMsg(struct evbuffer* buff);
		virtual int recvResponseMsg(struct evbuffer* buff, ResponseMsg& msg);

		virtual int sendResponseMsg(struct evbuffer* buff);
		virtual int recvRequestMsg(struct evbuffer* buff, RequestMsg& msg);

		std::vector<int8> recvbuff_;
		std::vector<int8> sendbuff_;
	};

	class HttpTransport :public ITransport {
	};
}
#endif