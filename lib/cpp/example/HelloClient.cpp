
#include <iostream>
#include "HelloMsg.h"
#include "rpc/NetEvent.h"
#include "rpc/RpcChannel.h"

struct timeval s10 = { 10,0 };
struct event *ev;

int main(int argc,char ** argv)
{
    std::string url("127.0.0.1:2619");
    rpc::EndPoint ep(url);

    rpc::RpcChannel<rpc::TcpTransport,rpc::BinaryProtocol> chan(rpc::NetEvent::getInstance(),ep);

    shuai::opServiceStub* client= chan.createStub<shuai::opServiceStub>();
    if (client==nullptr){
		std::cerr << "chan.createStub" << std::endl;
        return -1;
    }
	rpc::NetEvent::getInstance()->scheduleTimer([](evutil_socket_t fd, short what, void *arg) -> void{

	    shuai::opServiceStub* client=(shuai::opServiceStub*)arg;
	    client->login("shuai",[=](rpc::int8 a)->int{
	        std::cout<<a<<std::endl;
	        return 0;
	    });
	},client, timeval{5,0},timeval{10,0});

    rpc::NetEvent::getInstance()->eventLoop();

    return 0;
}