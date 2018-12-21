
#include <iostream>
#include "HelloMsg.h"
#include "Rpc/NetEvent.h"
#include "Rpc/RpcChannel.h"

int main(int argc,char ** argv)
{
    std::string url;

    rpc::RpcChannel chan(rpc::NetEvent::getInstance(),new Transport,new Protocol);
    HelloMsgStub client= chan.createStub<HelloMsgStub>(url) ;

    client.test(100,[](int a)->int{
        std::cout<<a<<std::endl;
        return 0;
    });


    rpc::NetEvent::getInstance().eventLoop();


    return 0;
}