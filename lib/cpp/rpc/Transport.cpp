#include "rpc/Transport.h"
#include "rpc/NetEvent.h"

int rpc::TcpTransport::sendRequestMsg(const RequestMsg & msg) {
    BinaryProtocol proto;
    msg.serialize(&proto);

	struct evbuffer * buff= evbuffer_new();
	uint16 len = uint16(proto.getBuffer().size());
    evbuffer_add(buff,(int8*)&len, 2);
    evbuffer_add(buff,proto.getBuffer().data(), proto.getBuffer().size());

	int ret = bufferevent_write_buffer(bev_, buff);
	evbuffer_free(buff);
	return ret;
}

int rpc::TcpTransport::recvResponseMsg(std::vector<int8>& buff, ResponseMsg & msg) {
    recvbuff_.insert(recvbuff_.end(), buff.begin(), buff.end());
    if (recvbuff_.size() <= 2) {
        return -1;
    }
    uint16 len = *(uint16*)recvbuff_.data();
    if (recvbuff_.size()<size_t(len + 2)) {
        return -1;
    }
	if (uint16(recvbuff_.size()) < len + 2) {
        return -1;
	}

    BinaryProtocol proto;
	proto.setBuffer(std::vector<int8>(recvbuff_.begin() + 2, recvbuff_.end()));
	msg.deSerialize(&proto);

    recvbuff_.insert(recvbuff_.begin(), recvbuff_.begin() + len + 2, recvbuff_.end());

    return 0;
}

int rpc::TcpTransport::sendResponseMsg(const ResponseMsg & msg) {

    BinaryProtocol proto;
    msg.serialize(&proto);

	struct evbuffer * buff= evbuffer_new();
	uint16 len = uint16(proto.getBuffer().size());
    evbuffer_add(buff,(int8*)&len, 2);
    evbuffer_add(buff,proto.getBuffer().data(), proto.getBuffer().size());

	int ret = bufferevent_write_buffer(bev_, buff);
	evbuffer_free(buff);
	return ret;
}

int rpc::TcpTransport::recvRequestMsg(std::vector<int8>& buff, RequestMsg & msg) {
    recvbuff_.insert(recvbuff_.end(), buff.begin(), buff.end());
    if (recvbuff_.size() <= 2) {
        return -1;
    }
    uint16 len = *(uint16*)recvbuff_.data();
    if (recvbuff_.size()<size_t(len + 2)) {
        return -1;
    }
	if (uint16(recvbuff_.size()) < len + 2) {
        return -1;
	}

    BinaryProtocol proto;
	proto.setBuffer(std::vector<int8>(recvbuff_.begin() + 2, recvbuff_.end()));
	msg.deSerialize(&proto);

    recvbuff_.insert(recvbuff_.begin(), recvbuff_.begin() + len + 2, recvbuff_.end());

    return 0;
}
