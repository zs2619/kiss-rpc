
#ifndef __RPC_SERVICEPROXY_H__
#define __RPC_SERVICEPROXY_H__

#include "rpc/Common.h"
#include "rpc/RpcMessage.h"
#include "rpc/EventHandler.h"
#include "rpc/RpcService.h"
namespace rpc {
class ServiceProxy:public EventHandler{
public:
	ServiceProxy(const Connection* service):service_(service){}
    virtual ~ServiceProxy() {}

protected:
    virtual int invoke(std::shared_ptr<RpcMsg> msg) {
        msg->responseMsg_.msgId=msg->requestMsg_.msgId;
        msg->responseMsg_.msgSeqId=msg->requestMsg_.msgSeqId;

        BinaryProtocol proto;
        msg->responseMsg_.serialize(&proto);

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

	    if (-1==service_->getTransport()->sendResponseMsg(buff)){
            return -1;
		}
        return 0;
    };

	virtual bool dispatch(std::shared_ptr<RpcMsg> m) { return true; };

protected:
    const Connection* service_;
};
}
#endif