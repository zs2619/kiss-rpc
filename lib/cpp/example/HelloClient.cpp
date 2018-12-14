
#include <iostream>
#include "HelloMsg.h"
#include "Rpc/RpcChannel.h"

int main(int argc,char ** argv)
{
    std::string url;
    RpcChannel chan(url,new Transport,new Protocol);
    HelloMsgStub client= chan.createStub<HelloMsgStub>() ;

    client.test(100,[](int a)->int{
        std::cout<<a<<std::endl;
        return 0;
    });

    while(1) {
        int ret=chan.dispatch();
        if (ret==-1){
            break;
        }
    }

    chan.terminate();

    return 0;
}