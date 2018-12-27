
#ifndef	__RPC_CLIENTSTUB_H__
#define	__RPC_CLIENTSTUB_H__

#include "RpcMessage.h"
#include "EventHandler.h"
namespace rpc {
class  ClientStub:public EventHandler{
public:
	ClientStub(){}
	virtual ~ClientStub() {}


 protected:
    virtual int handleInput(){
		RpcMsg m;
		dispatch(m);
	}
    virtual int handleOutput(){

	}
    virtual int handleClose(){

	}
};
}
#endif