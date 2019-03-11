
#ifndef	__RPC_CLIENTSTUB_H__
#define	__RPC_CLIENTSTUB_H__

#include "rpc/Common.h"
#include "rpc/RpcMessage.h"
#include "rpc/EventHandler.h"
#include "rpc/Connection.h"

namespace rpc {
class  ServiceStub:public EventHandler{
public:
	ServiceStub(const Connection* chan):chan_(chan){}
	virtual ~ServiceStub() {chan_=nullptr;}

	virtual int invoke(std::shared_ptr<RpcMsg> msg) {

		msg->requestMsg_.msgSeqId=maxMsgSeqId_++;
		msg->time_= std::chrono::system_clock::now();

        BinaryProtocol proto;
        msg->requestMsg_.serialize(&proto);

        RpcHeader header;
        header.version=0;
        header.msgType=MPT_Bin;
        header.serviceName=msg->serviceName_;
        header.bodyLen=uint16(proto.getBuffer().size());

        BinaryProtocol headerProto;
        header.serialize(&headerProto);

        struct evbuffer * buff= evbuffer_new();

	    uint16 len = uint16(proto.getBuffer().size());
        evbuffer_add(buff,(int8*)&len,MsgHeaderMaxSize );
        evbuffer_add(buff,headerProto.getBuffer().data(),len );
        evbuffer_add(buff,proto.getBuffer().data(), proto.getBuffer().size());

		int ret=chan_->getTransport()->sendRequestMsg(buff);
        if (ret==-1){
            std::cout<<"chan_->getTransport()->sendRequestMsg"<<std::endl;
        }

		MsgQueue_[msg->requestMsg_.msgSeqId]=msg;
	    evbuffer_free(buff);
		return 0; 
	};

    int stubMsgCallBack(ResponseMsg& respMsg){
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

private:
	int64 maxMsgSeqId_=0;
	std::unordered_map<int64,std::shared_ptr<RpcMsg>> MsgQueue_;
protected:
    const Connection* chan_;
};
}
#endif