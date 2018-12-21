
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}

class Transport{
public:
   virtual int open()=0;
   virtual int close()=0;

   virtual int sendRequestMsg(const RequestMsg* msg)=0;
private:
   int         connStatus_;
};

class TcpTransport :public Transport{
public:
   virtual int sendRequestMsg(const RequestMsg* msg){};
private:
    bufferevent* bev_;
};

class HttpTransport :public Transport{
};