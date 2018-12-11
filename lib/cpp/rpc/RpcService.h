
#include <string>
#include "NetEvent.h"
#include "EndPoint.h"
#include "Transport.h"
#include "Protocol.h"
#include "ServiceProxy.h"


namespace rpc {
template<typename E,typename T,typename P>
class RpcService{

public:
    RpcService(NetEvent* event,EndPoint ep){ }
    ~RpcService(){ } 

    int open(){
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);

        event_->evconnlistener(base_,&sin,this,makeServerhandler);
    } 

    int makeServiceHandler(E e) {

    }

    int close(){
    }
    int invoke(const RpcMsg* m){
        RequestMsg reqMsg;
        trans_->sendRequestMsg(reqMsg);
    }

    int dispacth(const RpcMsg* msg){
    }

private:
    NetEvent*   event_;
    T           trans_;
    P           proto_;
    EndPoint    ep_;

	struct evconnlistener *listener;
};
}