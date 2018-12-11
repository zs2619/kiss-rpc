
#include <iostream>
#include <string>
#include "HelloMsg.h"
#include "Rpc/RpcServer.h"

class  HelloMsgProxy :public HelloMsgProxyIF
{
 public:

	virtual std::pair<int,int> test(int a){}
	virtual std::pair<int,std::string> testMsg(std::string s){};

};
int main(int argc,char ** argv)
{
    rpc::RpcService<HelloMsgStub,TcpTransport,BinaryProtocol> service(rpc::NetEvent::getInstance());
    EndPoint ep(std::string(":2619"));
    service.open(ep);

    rpc::NetEvent::getInstance().eventLoop();
    return 0;
}