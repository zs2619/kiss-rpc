
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
		msg->responseMsg_.header.version=0;
		msg->responseMsg_.header.msgType=MPT_Bin;
		msg->responseMsg_.header.serviceName=msg->serviceName_;

		if (-1==service_->getTransport()->sendResponseMsg(msg->responseMsg_)){
			return -1;
		}
		return 0;
	};
public:
	virtual bool dispatch(std::shared_ptr<RpcMsg> m) { return true; };

protected:
	const Connection* service_;
};
}
#endif