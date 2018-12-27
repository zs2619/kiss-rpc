#ifndef		__RPC_ENDPOINT_H__
#define		__RPC_ENDPOINT_H__

#include <string>

namespace rpc {
class EndPoint{
public:
    EndPoint(const std::string& url):hostname_(url){
    }
    ~EndPoint(){}

    const char *getHostName () {
        return "";
    }
    int getPort() {
        return 0;
    }

    struct sockaddr_in getAddrIn(){
    }

 private:
    std::string hostname_; 
};
}
#endif 