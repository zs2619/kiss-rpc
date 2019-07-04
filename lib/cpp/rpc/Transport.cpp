#include "rpc/Transport.h"
#include "rpc/NetEvent.h"
#include "rpc/Connection.h"

const size_t MsgHeaderMaxSize=2;
void rpc::ITransport::conn_eventcb(bufferevent * bev, short events, void * userData) {
	ITransport* trans = (ITransport*)userData;
	trans->conn_->handleClose();
}

void rpc::ITransport::conn_readcb(bufferevent * bev, void * userData) {
	ITransport* trans = (ITransport*)userData;
	struct evbuffer *input = bufferevent_get_input(bev);

	trans->conn_->handleInput(input);
}
void rpc::ITransport::conn_writecb(bufferevent * bev, void * userData) {
	ITransport* trans = (ITransport*)userData;
}

int rpc::TcpTransport::sendRequestMsg( const RequestMsg& reqMsg) {
	BinaryProtocol proto;
	reqMsg.serialize(&proto);

	uint16 len = uint16(proto.getBuffer().size());
	struct evbuffer * msgbuff= evbuffer_new();
	evbuffer_add(msgbuff,(byte*)&len,MsgHeaderMaxSize);
	evbuffer_add(msgbuff,proto.getBuffer().data(), proto.getBuffer().size());

	int ret = bufferevent_write_buffer(bev_, msgbuff);

	evbuffer_free(msgbuff);

	return ret;
}

int rpc::TcpTransport::recvResponseMsg(struct evbuffer* buff,std::vector<ResponseMsg>& msgVec) {
	int ret=0;
	while(1){
		if (evbuffer_get_length(buff) <= MsgHeaderMaxSize) {
			return ret;
		}
		struct evbuffer_iovec v[1];
		ret=evbuffer_peek(buff, MsgHeaderMaxSize, NULL, v, 1);
		RpcAssert(ret==1);
		uint16 msgLen= *(uint16*)(v[0].iov_base);

		if (evbuffer_get_length(buff) < size_t(msgLen+MsgHeaderMaxSize)) {
			return ret;
		}
		evbuffer_drain(buff,MsgHeaderMaxSize);
		ret+=MsgHeaderMaxSize;

		std::unique_ptr<byte[]> buffArray = std::make_unique<byte[]>(msgLen);
		evbuffer_remove(buff, buffArray.get(), msgLen);
		ret+=msgLen;
		std::vector<byte> buffVec(buffArray.get(), buffArray.get() + msgLen);

		BinaryProtocol proto;
		proto.setBuffer(buffVec);
		ResponseMsg msg;
		msg.deSerialize(&proto);
		msgVec.push_back(msg);
	}

	return ret;
}

int rpc::TcpTransport::sendResponseMsg(const ResponseMsg& respMsg) {
	BinaryProtocol proto;
	respMsg.serialize(&proto);

	uint16 len = uint16(proto.getBuffer().size());
	struct evbuffer * msgbuff= evbuffer_new();
	evbuffer_add(msgbuff,(byte*)&len,MsgHeaderMaxSize);
	evbuffer_add(msgbuff,proto.getBuffer().data(), proto.getBuffer().size());

	int ret = bufferevent_write_buffer(bev_, msgbuff);

	evbuffer_free(msgbuff);

	return ret;
}

int rpc::TcpTransport::recvRequestMsg(struct evbuffer* buff,std::vector<RequestMsg>& msgVec) {
	int ret=0;
	while(1){
		if (evbuffer_get_length(buff) <= MsgHeaderMaxSize) {
			return ret;
		}
		struct evbuffer_iovec v[1];
		ret=evbuffer_peek(buff, MsgHeaderMaxSize, NULL, v, 1);
		RpcAssert(ret==1);
		uint16 msgLen= *(uint16*)(v[0].iov_base);

		if (evbuffer_get_length(buff) < size_t( msgLen+MsgHeaderMaxSize)) {
			return ret;
		}
		evbuffer_drain(buff,MsgHeaderMaxSize);
		ret+=MsgHeaderMaxSize;

		std::unique_ptr<byte[]> buffArray = std::make_unique<byte[]>(msgLen);
		evbuffer_remove(buff, buffArray.get(), msgLen);
		ret+=msgLen;
		std::vector<byte> buffVec(buffArray.get(), buffArray.get() + msgLen);

		BinaryProtocol proto;
		proto.setBuffer(buffVec);
		RequestMsg msg;
		msg.deSerialize(&proto);
		msgVec.push_back(msg);
	}

	return ret;
}
