#ifndef __RPC_ENDPOINT_H__
#define	__RPC_ENDPOINT_H__

#include <string>
#include "NetEvent.h"


namespace rpc {
class EndPoint{
public:
    EndPoint(const std::string& url):hostname_(url){
    }
    ~EndPoint(){}

    const char *getHostName () const {
        return "";
    }
    int getPort() const {
        return 0;
    }

    struct sockaddr_in* getAddrIn() const {
		return nullptr;
    }

 private:
    std::string hostname_; 
};
}
#endif 