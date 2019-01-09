
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