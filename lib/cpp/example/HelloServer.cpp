
#include <iostream>
#include <string>
#include "HelloMsg.h"
#include "rpc/RpcService.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"

class  HelloMsgProxy :public HelloMsgProxyIF
{
 public:

	virtual std::pair<int,int> test(int a){}
	virtual std::pair<int,std::string> testMsg(std::string s){};

};
int main(int argc,char ** argv)
{
    rpc::EndPoint ep(std::string(":2619"));
    rpc::RpcService<HelloMsgProxy,TcpTransport,BinaryProtocol> service(rpc::NetEvent::getInstance(),ep);
    service.open();

    rpc::NetEvent::getInstance()->eventLoop();
    return 0;
}