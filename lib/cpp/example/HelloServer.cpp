
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
    RpcServer server(std::string(":2619"),new Transport,new Protocol);
    server.registerService(new HelloMsgProxy);

    server.run();
    return 0;
}