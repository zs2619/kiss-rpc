
#include <string>
#include "Transport.h"
#include "RpcMessage.h"
#include "Protocol.h"

class RpcChannel{
public:
    RpcChannel(const std::string& url,Transport* trans,Protocol* proto){
    }
    ~RpcChannel(){ }

    template<typename T>
    T createStub(){
        T t;
        return t;
    }
    int sendAsyncRpcMsg(RpcMsg* msg){
        return 0;
    }

    Protocol*  getProtocol(){return nullptr;}
    Transport* getTransport(){return nullptr;}

private:

    Transport*        trans_;
    Protocol*         proto_;
    std::string       url_;
};