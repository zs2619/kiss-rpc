
#ifndef __RPC_RPCSERVICE_H__
#define __RPC_RPCSERVICE_H__

#include <string>
#include "rpc/Common.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"
#include "rpc/Connection.h"

namespace rpc {
template<typename T,typename P>
class RpcService:public Connection ,public EventHandler{

public:
    RpcService(){ }

    ~RpcService(){ 
    } 

    static RpcService*  makeServiceHandler(struct bufferevent * be,std::map<const char* ,ServiceProxy> proxyMap) {
        RpcService*  service=new RpcService<T,P>();
        service->setHandler();
        service->init(std::make_shared<T>(),std::make_shared<P>());
        return service;
    }

	virtual bool dispatch(std::shared_ptr<RpcMsg> m){
        return true;

    };
    virtual int invoke(std::shared_ptr<RpcMsg> msg) {
        return 0;
    };

    virtual int handleInput(std::vector<int8>& buff){
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


    std::map<const char* ,ServiceProxy*> proxyMap_;
};
}
#endif