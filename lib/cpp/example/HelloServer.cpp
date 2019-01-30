
#include <iostream>
#include <string>
#include "HelloMsg.h"
#include "rpc/RpcService.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"

class  HelloMsgProxy :public shuai::opServiceProxyIF
{
 public:


	virtual std::tuple<int>momo(rpc::int8  i8, rpc::int64  i64) {
	  return std::make_tuple(1); 
	};
	virtual std::tuple<int, rpc::int8>xixi(shuai::user&  u) {
	  return std::make_tuple(1,1); 
	};
	virtual std::tuple<int, std::vector<shuai::role> >lala(std::map<rpc::int32, shuai::role>&  m, std::vector<rpc::int32> &  ai, std::vector<shuai::role> &  ar) {
	  std::vector<shuai::role> roles;
	  return std::make_tuple(1,roles); 
	};
	virtual std::tuple<int, rpc::int8>ping() {
	  return std::make_tuple(1,1); 
	};

};
int main(int argc,char ** argv)
{
    rpc::EndPoint ep(std::string(":2619"));
    rpc::RpcService<HelloMsgProxy,rpc::TcpTransport,rpc::BinaryProtocol> service(rpc::NetEvent::getInstance(),ep);
	if (-1 == service.open()) {
		std::cerr << "service.open " << std::endl;
	}

    int ret=rpc::NetEvent::getInstance()->eventLoop();
    return 0;
}