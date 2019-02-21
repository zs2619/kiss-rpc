
#ifndef	__RPC_CLIENTSTUB_H__
#define	__RPC_CLIENTSTUB_H__

#include <unordered_map>
#include <iostream>
#include "RpcMessage.h"
#include "EventHandler.h"
namespace rpc {
class  ClientStub:public EventHandler{
public:
	ClientStub(){}
	virtual ~ClientStub() {}

	virtual int invoke(std::shared_ptr<RpcMsg> msg) {
		msg->requestMsg_.msgSeqId=maxMsgSeqId_++;
		msg->time_= std::chrono::system_clock::now();
		MsgQueue_[msg->requestMsg_.msgSeqId]=msg;
		getTransport()->sendRequestMsg(msg->requestMsg_);
		return 0; 
	};

    virtual int handleInput(std::vector<int8>& buff){
		ResponseMsg respMsg;
		if (-1==getTransport()->recvResponseMsg(buff,respMsg)){
			return 0;
		}

		auto it = MsgQueue_.find(respMsg.msgSeqId);
		if (it == MsgQueue_.end()) {
			return -1;
		}
		std::shared_ptr<RpcMsg> msg=it->second;
		msg->responseMsg_ = respMsg;
		dispatch(msg);
		MsgQueue_.erase(it);
	
		return 0;
	}
    virtual int handleOutput(){
		return 0;

	}
    virtual int handleClose(){
		std::cout<<"handleClose"<<std::endl;
		return 0;
	}
private:
	int64 maxMsgSeqId_=0;
	std::unordered_map<int64,std::shared_ptr<RpcMsg>> MsgQueue_;
};
}
#endif