
#include <string>
#include "EndPoint.h"
#include "Transport.h"
#include "NetEvent.h"
#include "RpcMessage.h"
#include "Protocol.h"
#include "ClientStub.h"


namespace rpc {
class RpcChannel{
public:
    RpcChannel(NetEvent* event,const std::string& url,Transport* trans,Protocol* proto){
    }
    ~RpcChannel(){
        terminate();
     }

    template<typename T>
    T createStub(){
        T t;
        stub_.push_back(t);
        return t;
    }

    int invoke(const RpcMsg* msg){
        RequestMsg reqMsg;
        trans_->sendRequestMsg(reqMsg);
        return 0;
    }

    int dispacth(const RpcMsg* msg){ }

    int open(){
        trans_->open();
    }

    int close(){
        trans_->close();
    }
    Transport* makeServerhandler(bufferevent *bev){

    }


    Protocol*  getProtocol(){return nullptr;}
    Transport* getTransport(){return nullptr;}


private:
    std::vector<ClientStub*> stub_;
    NetEvent*         event_;
    Transport*        trans_;
    Protocol*         proto_;
    EndPoint          ep_;
};
}