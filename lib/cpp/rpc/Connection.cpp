
#include "rpc/Connection.h"
void rpc::Connection::conn_eventcb(bufferevent * bev, short events, void * userData) {
	Connection* conn = (Connection*)userData;
	conn->handleClose();
}

void rpc::Connection::conn_readcb(bufferevent * bev, void * userData) {
	Connection* conn = (Connection*)userData;
	struct evbuffer *input = bufferevent_get_input(bev);
	conn->handleInput(input);
}
void rpc::Connection::conn_writecb(bufferevent * bev, void * userData) {
	Connection* conn = (Connection*)userData;
	conn->handleOutput();
}
