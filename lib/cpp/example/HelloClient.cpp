
#include <iostream>
#include "HelloMsg.h"
#include "Rpc/NetEvent.h"
#include "Rpc/RpcChannel.h"

int main(int argc,char ** argv)
{
    std::string url;
    EndPoint ep(url);

    rpc::RpcChannel<HelloMsgStub,TcpTransport,BinaryProtocol> chan(rpc::NetEvent::getInstance());

    HelloMsgStub* client=nullptr;
    chan.createStub(client,ep) ;

    client.test(100,[](int a)->int{
        std::cout<<a<<std::endl;
        return 0;
    });


    rpc::NetEvent::getInstance().eventLoop();


    return 0;
}