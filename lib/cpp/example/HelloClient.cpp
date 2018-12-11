
#include <iostream>
#include "HelloMsg.h"
#include "Rpc/RpcChannle.h"

int main(int argc,char ** argv)
{
    RpcChannle chan(new Transport,new Protocol);
    HelloMsgStub client= chan.createStub<HelloMsgStub>() ;

    client.test(100,[](int a)->int{
        std::cout<<a<<std::endl;
        return 0;
    });

    return 0;
}