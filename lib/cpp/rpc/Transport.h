
#ifndef	__RPC_TRANSPORT_H__
#define	__RPC_TRANSPORT_H__
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}
#include "RpcMessage.h"
namespace rpc {
	class ITransport {
	public:
		ITransport():bev_(nullptr),connStatus_(0){}
		virtual ~ITransport(){}

		void setBufferEvent(bufferevent* bev) {
			bev_ = bev;
		}

		virtual int sendRequestMsg(const RequestMsg& msg) = 0;
		virtual int recvResponseMsg(std::vector<int8>& buff, ResponseMsg& msg) = 0;

		virtual int sendResponseMsg(const ResponseMsg& msg) = 0;
		virtual int recvRequestMsg(std::vector<int8>& buff, RequestMsg& msg) = 0;

	protected:
		int         connStatus_;
		bufferevent* bev_;
	};

	class TcpTransport :public ITransport {
	public:
		virtual int sendRequestMsg(const RequestMsg& msg);
		virtual int recvResponseMsg(std::vector<int8>& buff, ResponseMsg& msg);

		virtual int sendResponseMsg(const ResponseMsg& msg);
		virtual int recvRequestMsg(std::vector<int8>& buff, RequestMsg& msg);

		std::vector<int8> recvbuff_;
		std::vector<int8> sendbuff_;
	};

	class HttpTransport :public ITransport {
	};
}
#endif