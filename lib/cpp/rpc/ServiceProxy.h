
#ifndef __RPC_SERVICEPROXY_H__
#define __RPC_SERVICEPROXY_H__

#include "RpcMessage.h"
#include "EventHandler.h"
namespace rpc {
class ServiceProxy:public EventHandler{
public:
	ServiceProxy(){}
	virtual ~ServiceProxy() {}

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