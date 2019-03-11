
#ifndef __RPC_RPCSERVICE_H__
#define __RPC_RPCSERVICE_H__

#include <string>
#include "rpc/Common.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"
#include "rpc/Connection.h"

namespace rpc {
	class ServiceProxy;
template<typename T,typename P,typename ... Types>
class RpcService:public Connection  {

public:
    RpcService(NetEvent*  event,EndPoint ep, struct bufferevent* bev)
    :Connection(event,ep,bev,new T(),new P()){ 
    }

    ~RpcService(){ 
    } 

   template<typename N>
   static  void*  createProxy(RpcService<T,P>* conn) {
	   conn->proxyMap_[N::getObjName] = new N(conn);
	   return nullptr;
   }

    static RpcService<T, P>*  makeServiceHandler(NetEvent*   event,EndPoint   ep, struct bufferevent* bev) {
        RpcService<T,P>*  service=new RpcService<T,P>(event,ep,bev);
		service->handleConnction();
        /*神奇代码 动态创建 模板不定参数类型 对象*/
        auto f= { createProxy<Types>(service)... };
        return service;
    }

    virtual int handleInput(struct evbuffer* buff){
	    RequestMsg reqMsg;
	    if (-1==getTransport()->recvRequestMsg(buff,reqMsg)){
			return -1;
		}
 	    std::shared_ptr<RpcMsg> msg = std::make_shared<RpcMsg>();
		msg->requestMsg_.msgId = reqMsg.msgId;
		msg->requestMsg_.msgSeqId = reqMsg.msgSeqId;
		msg->requestMsg_.buf = reqMsg.buf;
		//dispatch(msg);
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
        setHandler();
    	return 0;
    };



    std::map<const char* ,ServiceProxy*> proxyMap_;
};
}
#endif