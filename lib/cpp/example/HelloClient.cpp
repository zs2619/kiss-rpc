
#include <iostream>
#include "HelloMsg.h"
#include "rpc/NetEvent.h"
#include "rpc/RpcChannel.h"

int main(int argc,char ** argv)
{
    std::string url("127.0.0.1:2619");
    rpc::EndPoint ep(url);

    rpc::RpcChannel<HelloMsgStub,TcpTransport,BinaryProtocol> chan(rpc::NetEvent::getInstance());

    HelloMsgStub* client=nullptr;
    chan.createStub(client,ep) ;

    client->test(100,[](int a)->int{
        std::cout<<a<<std::endl;
        return 0;
    });


    rpc::NetEvent::getInstance()->eventLoop();


    return 0;
}