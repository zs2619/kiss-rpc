
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}

namespace rpc {
class NetEvent {
  typedef  void NetEventListenerCb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *sa, int socklen, void *user_data)

 public:
    NetEvent(){
    #ifdef _WIN32
        WSADATA wsa_data;
        WSAStartup(0x0201, &wsa_data);
    #endif
    	base_ = event_base_new();
    }

    ~NetEvent(){
	    event_base_free(getEventBase());
    }
    static NetEvent* getInstance(){}


    struct evconnlistener * evconnlistener(NetEvent* event,struct sockaddr* sin  void *userData,NetEventListenerCb cb){
	   struct evconnlistener * listener = evconnlistener_new_bind(base, listener_cb, (void *)userData,
	    LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
	    (struct sockaddr*)sin,
	    sizeof(*sin));
        return listener;
    } 

    int eventLoop(){
	    event_base_dispatch(getEventBase());
    }

    int endEventLoop(){
        event_base_loopbreak(getEventBase());
    }

    struct event_base*  getEventBase(){base_;}

private:
    static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *sa, int socklen, void *user_data)
    {
        RpcService* service=(RpcService*)user_data

        struct bufferevent *bev;
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if (!bev) {
            fprintf(stderr, "Error constructing bufferevent!");
            event_base_loopbreak(base);
            return;
        }
        Transport* trans= service->makeServerhandler(bev);
        bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, trans);
        bufferevent_enable(bev, EV_WRITE|EV_READ);
    }

    static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
    {
    }

    static void conn_readcb(struct bufferevent *bev, void *ctx) { 
    }

    static void conn_writecb(struct bufferevent *bev, void *ctx) { 
    }

	struct event_base*  base_;
};
}