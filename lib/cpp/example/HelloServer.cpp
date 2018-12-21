
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
    rpc::RpcService service(rpc::NetEvent::getInstance(),std::string(":2619"),new Transport,new Protocol);
    service.registerService(new HelloMsgProxy);
    service.open();

    rpc::NetEvent::getInstance().eventLoop();
    return 0;
}