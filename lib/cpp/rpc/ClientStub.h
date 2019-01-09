
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
	virtual int invoke(const RpcMsg* msg) { return 0; };
    virtual int handleInput(){
		RpcMsg m;
		dispatch(m);
		return 0;
	}
    virtual int handleOutput(){
		return 0;

	}
    virtual int handleClose(){
		return 0;

	}
};
}
#endif