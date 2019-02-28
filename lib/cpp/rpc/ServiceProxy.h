
#ifndef __RPC_SERVICEPROXY_H__
#define __RPC_SERVICEPROXY_H__

#include "rpc/Common.h"
#include "rpc/RpcMessage.h"
#include "rpc/EventHandler.h"
namespace rpc {
class ServiceProxy:public EventHandler{
public:
	ServiceProxy(){}
    virtual ~ServiceProxy() {}

protected:
    virtual int invoke(std::shared_ptr<RpcMsg> msg) {
        msg->responseMsg_.msgId=msg->requestMsg_.msgId;
        msg->responseMsg_.msgSeqId=msg->requestMsg_.msgSeqId;
	    if (-1==getTransport()->sendResponseMsg(msg->responseMsg_)){
            return -1;
		}
        return 0;
    };

    virtual int handleInput(std::vector<int8>& buff){
	    RequestMsg reqMsg;
	    if (-1==getTransport()->recvRequestMsg(buff,reqMsg)){
			return -1;
		}
 	    std::shared_ptr<RpcMsg> msg = std::make_shared<RpcMsg>();
		msg->requestMsg_.msgId = reqMsg.msgId;
		msg->requestMsg_.msgSeqId = reqMsg.msgSeqId;
		msg->requestMsg_.buff = reqMsg.buff;
		dispatch(msg);
		return 0;
	}

    virtual int handleOutput(){
		return 0;
	}
    virtual int handleClose(){
		std::cout<<"handleClose"<<std::endl;
		return 0;
	}
    virtual int handleConnction(){
    	return 0;
    };
};
}
#endif