
#ifndef __RPC_EVENTHANDLER_H__
#define __RPC_EVENTHANDLER_H__

#include "rpc/RpcMessage.h"
#include "rpc/NetEvent.h"
namespace rpc {

class EventHandler{

public:
    EventHandler(){}
    virtual ~EventHandler(){ }

    virtual bool dispatch(std::shared_ptr<RpcMsg> m)=0;
    virtual int invoke(std::shared_ptr<RpcMsg> msg)=0;
};

}
#endif