
#include <string>
#include "NetEvent.h"
#include "EndPoint.h"
#include "Transport.h"
#include "Protocol.h"
#include "ServiceProxy.h"


namespace rpc {
class RpcService{

public:
    RpcService(NetEvent* event,const std::string& url,Transport* trans,Protocol* proto){ }
    ~RpcService(){ } 

    template<typename T>
    bool registerService(ServiceProxy* proxy){
        proxyArray_.push_back(proxy);
    }

    int open(){
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);

        event_->evconnlistener(base_,&sin,this,makeServerhandler);
    } 

    Transport* makeServerhandler(bufferevent *bev){
       RpcService* service=(RpcService*)userData;
       return Transport;
    }

    int close(){
    }
    int invoke(const RpcMsg* m){
        RequestMsg reqMsg;
        trans_->sendRequestMsg(reqMsg);
    }

    int dispacth(){
    }

private:
    NetEvent*         event_;
    Transport*        trans_;
    EndPoint          ep_;

	struct evconnlistener *listener;

    std::vector<ServiceProxy*> proxyArray_;
};
}