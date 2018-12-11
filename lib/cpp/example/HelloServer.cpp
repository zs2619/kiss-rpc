
#include <iostream>
#include <string>
#include "HelloMsg.h"
#include "Rpc/RpcServer.h"

int main(int argc,char ** argv)
{
    RpcServer server(std::string(":2619"),new Transport,new Protocol);
    server.registerService(new HelloMsgProxy);

    server.run();
}