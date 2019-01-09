
#include <iostream>
#include <string>
#include "HelloMsg.h"
#include "rpc/RpcService.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"

class  HelloMsgProxy :public HelloMsgProxyIF
{
 public:

	virtual std::pair<int, int> test(int a) { return std::pair<int, int>(); }
	virtual std::pair<int,std::string> testMsg(std::string s){return std::pair<int, std::string>(); };

};
int main(int argc,char ** argv)
{
    rpc::EndPoint ep(std::string(":2619"));
    rpc::RpcService<HelloMsgProxy,TcpTransport,BinaryProtocol> service(rpc::NetEvent::getInstance(),ep);
	if (-1 == service.open()) {
		std::cerr << "service.open " << std::endl;
	}

    int ret=rpc::NetEvent::getInstance()->eventLoop();
    return 0;
}