
#include <iostream>
#include "HelloMsg.h"
#include "rpc/NetEvent.h"
#include "rpc/RpcChannel.h"

struct timeval s10 = { 10,0 };
struct event *ev;

static void timer_callback(evutil_socket_t fd, short what, void *arg){

    HelloMsgStub* client=(HelloMsgStub*)arg;
    client->test(100,[](int a)->int{
        std::cout<<a<<std::endl;
        return 0;
    });
}
int main(int argc,char ** argv)
{
    std::string url("127.0.0.1:2619");
    rpc::EndPoint ep(url);

    rpc::RpcChannel<HelloMsgStub,TcpTransport,BinaryProtocol> chan(rpc::NetEvent::getInstance());

    HelloMsgStub* client=nullptr;
    int ret=chan.createStub(client,ep);
    if (ret==-1){
		std::cerr << "chan.createStub" << std::endl;
        return -1;
    }
	ev = event_new(rpc::NetEvent::getInstance()->getEventBase(), -1, EV_PERSIST, timer_callback, client);
	event_add(ev ,&s10); 

    rpc::NetEvent::getInstance()->eventLoop();

    return 0;
}