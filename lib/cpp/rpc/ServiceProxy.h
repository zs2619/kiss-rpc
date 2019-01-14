
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
    virtual int invoke(std::shared_ptr<RpcMsg> msg) {
	    if (-1==getTransport()->sendResponseMsg(msg->recvMsg_)){
		}
        return 0;
    };

    virtual int handleInput(std::vector<int8>& buff){
	    RequestMsg reqMsg;
	    if (-1==getTransport()->recvRequestMsg(buff,reqMsg)){
		}

 	    std::shared_ptr<RpcMsg> msg = std::make_shared<RpcMsg>();
		dispatch(msg);
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