
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
}
#include <string>
#include "Transport.h"
#include "Protocol.h"
#include "ServiceProxy.h"

class RpcServer{

public:
    RpcServer(const std::string& url,Transport* trans,Protocol* proto){

    #ifdef _WIN32
        WSADATA wsa_data;
        WSAStartup(0x0201, &wsa_data);
    #endif
        base_ = event_base_new();
    }

    ~RpcServer(){
    }

    bool registerService(HelloMsgProxyIF* proxy){
        proxyArray_.push_back(proxy);
    }

    bool run(){
	    event_base_dispatch(base_);
        return true;
    }

    int init(){

        struct sockaddr_in sin;

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(2619);

        listener_ = evconnlistener_new_bind(base_, listener_cb, (void *)this,
            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
            (struct sockaddr*)&sin,
            sizeof(sin));

        if (!listener_) {
            fprintf(stderr, "Could not create a listener!\n");
            return 1;
        }
        return 0;
    }

    int term(){
	    evconnlistener_free(listener_);
	    event_base_free(base_);
        return 0;
    }

    static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *sa, int socklen, void *user_data)
    {
        struct event_base *base = ((RpcServer*)user_data)->base_;
        struct bufferevent *bev;

        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if (!bev) {
            fprintf(stderr, "Error constructing bufferevent!");
            event_base_loopbreak(base);
            return;
        }
        bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
        bufferevent_enable(bev, EV_WRITE|EV_READ);
        return;
    }

    static void conn_writecb(struct bufferevent *bev, void *user_data) {
    }
    static void conn_readcb(struct bufferevent *bev, void *user_data) {
    }

    static void conn_eventcb(struct bufferevent *bev, short events, void *user_data) {
    }

    static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
    {
        if (events & BEV_EVENT_EOF) {
            printf("Connection closed.\n");
        } else if (events & BEV_EVENT_ERROR) {
            printf("Got an error on the connection: %s\n",
                strerror(errno));/*XXX win32*/
        }
        /* None of the other events can happen here, since we haven't enabled
        * timeouts */
        bufferevent_free(bev);
    }


private:

	struct event_base *base_;
	struct evconnlistener *listener_;
    std::vector<HelloMsgProxyIF*> proxyArray_;
    std::string url_;
};