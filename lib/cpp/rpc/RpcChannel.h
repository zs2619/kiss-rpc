
#include <string>
#include "EndPoint.h"
#include "Transport.h"
#include "NetEvent.h"
#include "RpcMessage.h"
#include "Protocol.h"
#include "ClientStub.h"


namespace rpc {
template<typename E,typename T,typename P>
class RpcChannel{
public:
    RpcChannel(NetEvent* event){
    }
    ~RpcChannel(){
        terminate();
     }

    int createStub(EventHandler * eh,EndPoint ep){
        struct bufferevent* be= event_->connect(this);
        return 0;
    }

    int makeServiceHandler(EventHandler* e,struct bufferevent * be) {
        handler=new E(new T(),new P());
        stubMap_[ep]=handler;
        eh=handler;
    }

private:
    std::map<std::sting,EventHandler*> stubMap_;
    NetEvent*  event_;
};
}