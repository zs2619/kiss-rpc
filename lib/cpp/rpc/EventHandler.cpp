#include "EventHandler.h"

void rpc::EventHandler::conn_eventcb(bufferevent * bev, short events, void * userData) {
    EventHandler* handler = (EventHandler*)userData;
    handler->handleClose();
}

void rpc::EventHandler::conn_readcb(bufferevent * bev, void * userData) {
    EventHandler* handler = (EventHandler*)userData;


    struct evbuffer *input = bufferevent_get_input(bev);
    size_t n = evbuffer_get_length(input);

    std::unique_ptr< int8[]> buff = std::make_unique<int8[]>(n);

    size_t size = bufferevent_read(bev, buff.get(), n);

    std::vector<int8> buffVec(buff.get(), buff.get() + size);
    handler->handleInput(buffVec);
}
void rpc::EventHandler::conn_writecb(bufferevent * bev, void * userData) {
    EventHandler* handler = (EventHandler*)userData;
    handler->handleOutput();
}
