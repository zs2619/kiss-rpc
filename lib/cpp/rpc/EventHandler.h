
#ifndef __RPC_EVENTHANDLER_H__
#define __RPC_EVENTHANDLER_H__

#include "rpc/RpcMessage.h"
#include "rpc/NetEvent.h"
#include "rpc/Transport.h"
#include "rpc/Protocol.h"
namespace rpc {

class EventHandler{

 public:
    EventHandler(){}
    virtual ~EventHandler(){
        trans_=nullptr;
        proto_=nullptr;
    }

    int init(std::shared_ptr<ITransport> trans,std::shared_ptr<IProtocol> proto){
        trans_= trans;
        proto_= proto;
		return 0;
    }
    int fini(){
        delete this;
		return 0;
    }
    virtual std::string getObjName(){return "";}
	virtual bool dispatch(std::shared_ptr<RpcMsg> m)=0;
    virtual int invoke(std::shared_ptr<RpcMsg> msg)=0;

    virtual int handleInput(std::vector<int8>& buff)=0;
    virtual int handleOutput()=0;
    virtual int handleClose()=0;
    virtual int handleConnction()=0;

    IProtocol* getProtocol(){return proto_.get();}
    ITransport* getTransport(){return trans_.get();}


private:
    std::shared_ptr<ITransport>   trans_;
    std::shared_ptr<IProtocol>    proto_;

};

}
#endif