
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

		virtual int sendRequestMsg(const RequestMsg& reqMsg) = 0;
		virtual int recvResponseMsg(struct evbuffer* buff, std::vector<ResponseMsg>& msg) = 0;

		virtual int sendResponseMsg(const ResponseMsg& respMsg) = 0;
		virtual int recvRequestMsg(struct evbuffer* buff, std::vector<RequestMsg>& msgVec) = 0;

	protected:
		int         connStatus_;
		bufferevent* bev_;
	};

	class TcpTransport :public ITransport {
	public:
		TcpTransport(){}
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