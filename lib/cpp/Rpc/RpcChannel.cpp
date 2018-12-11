
#include <string>
#include "Transport.h"
#include "Protocol.h"

class RpcChannel{
public:
    RpcChannel(const std::string& url,Transport* trans,Protocol* proto){
    }
    ~RpcChannel(){

    }
    template<typename T>
    T createStub(){
        T t;
        return t;
    }

private:
    Transport*        trans_;
    Protocol*         proto_;
    const std::string url_;
};