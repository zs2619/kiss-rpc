
#include <string>

extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}

#include "Transport.h"
#include "RpcMessage.h"
#include "Protocol.h"

class RpcChannel{
public:
    RpcChannel(const std::string& url,Transport* trans,Protocol* proto){
        #ifdef _WIN32
            WSADATA wsa_data;
            WSAStartup(0x0201, &wsa_data);
        #endif
    	base_ = event_base_new();
    }
    ~RpcChannel(){
        terminate();
     }

    template<typename T>
    T createStub(){
        T t;
        return t;
    }
    int sendAsyncRpcMsg(const RpcMsg* msg){
        return 0;
    }

    int start(){

        int be_flags=BEV_OPT_CLOSE_ON_FREE;
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(2619);
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	    evconn_ = bufferevent_socket_new(base_, -1, be_flags);
        bufferevent_setcb(evconn_, nullptr,nullptr, reader_eventcb, this);
		int ret = bufferevent_socket_connect_hostname(evconn_, nullptr, AF_INET, "127.0.0.1", 2619);
    }

    int stop(){
        bufferevent_disable(evconn_,EV_WRITE|EV_READ);
        bufferevent_setcb(evconn_, nullptr,nullptr, nullptr, getEventBase());
        event_base_loopbreak(getEventBase());
    }

    int dispatch(int time=0){
	    event_base_dispatch(getEventBase());
    }
    void terminate(){
        stop();
	    event_base_free(getEventBase());
    }

    Protocol*  getProtocol(){return nullptr;}
    Transport* getTransport(){return nullptr;}
    struct event_base*  getEventBase(){base_;}

private:

    static void reader_eventcb(struct bufferevent *bev, short what, void *ctx) { 
        RpcChannel* chan=(RpcChannel*)ctx;
	    bufferevent_enable(bev,EV_WRITE|EV_READ );
        bufferevent_setcb(bev, conn_readcb,conn_writecb, nullptr, chan->getEventBase());
    }

    static void conn_readcb(struct bufferevent *bev, void *ctx) { 
        RpcChannel* chan=(RpcChannel*)ctx;
        Transport* trans= chan->getTransport();
    }

    static void conn_writecb(struct bufferevent *bev, void *ctx) { 
        RpcChannel* chan=(RpcChannel*)ctx;
        Transport* trans= chan->getTransport();
    }

private:
	struct event_base*  base_;
    struct bufferevent* evconn_;
    std::vector<ClientStub*> stub_;
    Transport*        trans_;
    Protocol*         proto_;
    std::string       url_;
};