
#ifndef __RPC_CONNECTION_H__
#define __RPC_CONNECTION_H__
#include "rpc/NetEvent.h"
#include "rpc/Common.h"
#include "rpc/Transport.h"
#include "rpc/Protocol.h"
namespace rpc {
class Connection
{
public:
	Connection(NetEvent*  event,EndPoint ep ,ITransport* trans,IProtocol* proto):
	event_(event),
	ep_(ep),
	trans_(trans),
	proto_(proto){
	}
	~Connection(){
		trans_=nullptr;
		proto_=nullptr;
		event_=nullptr;
	}

	virtual int handleConnction(struct bufferevent* bev)=0;
	virtual int handleInput(struct evbuffer *buff)=0;
	virtual int handleClose()=0;

	IProtocol* getProtocol() const {return proto_;}
	ITransport* getTransport()const {return trans_;}

private:
	ITransport*   trans_;
	IProtocol*    proto_;

protected:
	NetEvent*             event_; 
	EndPoint              ep_;
};
    
    
}
#endif