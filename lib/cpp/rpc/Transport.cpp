#include "rpc/Transport.h"
#include "rpc/NetEvent.h"

int rpc::TcpTransport::sendRequestMsg( struct evbuffer* buff) {
	int ret = bufferevent_write_buffer(bev_, buff);
	return ret;
}

int rpc::TcpTransport::recvResponseMsg(struct evbuffer* buff, ResponseMsg & msg) {

    if (evbuffer_get_length(buff) <= 2) {
        return -1;
    }

    return 0;
}

int rpc::TcpTransport::sendResponseMsg(struct evbuffer* buff) {
	
	int ret = bufferevent_write_buffer(bev_, buff);
	evbuffer_free(buff);
	return ret;
}

int rpc::TcpTransport::recvRequestMsg(struct evbuffer* buff, RequestMsg & msg) {


    return 0;
}
