
#include <string>
#include "Transport.h"
#include "Protocol.h"
#include "ServiceProxy.h"

class RpcServer{

public:
    RpcServer(const std::string& url,Transport* trans,Protocol* proto){
    }
    ~RpcServer(){
    }
    bool registerService(HelloMsgProxyIF* proxy){

    }

    bool run(){
    }

private:

    const std::string url_;
};