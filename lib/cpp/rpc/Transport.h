
#ifndef	__RPC_TRANSPORT_H__
#define	__RPC_TRANSPORT_H__
extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
}
#include "RpcMessage.h"

class Transport{
public:


   virtual int sendRequestMsg(const RequestMsg* msg)=0;
   virtual int recvResponseMsg(const ResponseMsg* msg)=0;

   virtual int sendResponseMsg(const ResponseMsg* msg)=0;
   virtual int recvRequestMsg(const RequestMsg* msg)=0;

private:
   int         connStatus_;
};

class TcpTransport :public Transport{
public:
   virtual int sendRequestMsg(const RequestMsg* msg){
	   return 0;

   };
   virtual int recvResponseMsg(const ResponseMsg* msg){

	   return 0;
   };

   virtual int sendResponseMsg(const ResponseMsg* msg){

	   return 0;
   };
   virtual int recvRequestMsg(const RequestMsg* msg){
	   return 0;
   };
private:
    bufferevent* bev_;
};

class HttpTransport :public Transport{
};
#endif