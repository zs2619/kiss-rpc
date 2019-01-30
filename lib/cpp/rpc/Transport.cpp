#include "rpc/Transport.h"

int rpc::TcpTransport::sendRequestMsg(const RequestMsg & msg) {
    return bufferevent_write(bev_, msg.buf.data(), msg.buf.size());
}

int rpc::TcpTransport::recvResponseMsg(std::vector<int8>& buff, ResponseMsg & msg) {
    buff_.insert(buff_.end(), buff.begin(), buff.end());
    if (buff_.size() <= 2) {
        return -1;
    }
    uint16 len = *(uint16*)buff_.data();
    if (buff_.size()<size_t(len + 2)) {
        return -1;
    }

    buff_.insert(buff_.begin(), buff_.begin() + len + 2, buff_.end());
    return 0;
}

int rpc::TcpTransport::sendResponseMsg(const ResponseMsg & msg) {
    return bufferevent_write(bev_, msg.buf.data(), msg.buf.size());
}

int rpc::TcpTransport::recvRequestMsg(std::vector<int8>& buff, RequestMsg & msg) {
    buff_.insert(buff_.end(), buff.begin(), buff.end());
    if (buff_.size() <= 2) {
        return -1;
    }
    uint16 len = *(uint16*)buff_.data();
    if (buff_.size()<size_t(len + 2)) {
        return -1;
    }


    buff_.insert(buff_.begin(), buff_.begin() + len + 2, buff_.end());

    return 0;
}
