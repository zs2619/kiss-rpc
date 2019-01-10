#ifndef __RPC_ENDPOINT_H__
#define	__RPC_ENDPOINT_H__

#include <string>
#include <sstream>
#include "NetEvent.h"
	

namespace rpc {
class EndPoint{
public:
	EndPoint(const std::string& url);
    ~EndPoint(){}

    const char *getHostName () const {
        return hostname_.c_str();
    }

    int getPort() const {
        return ntohs(sockAddrIn_.sin_port);
    }

    const struct sockaddr_in* getAddrIn() const {
        return  &sockAddrIn_;
    }

	int set(const std::string& url);
    

 private:
   struct sockaddr_in sockAddrIn_;
   std::string hostname_; 
};
}
#endif 