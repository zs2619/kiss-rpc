
#include <iostream>
#include <string>
#include "HelloMsg.h"
#include "rpc/RpcServer.h"
#include "rpc/NetEvent.h"
#include "rpc/EndPoint.h"

class  HelloMsgProxy :public shuai::opServiceProxyIF
{
 public:
	HelloMsgProxy(const rpc::Connection* conn=nullptr):opServiceProxyIF(conn){}

	virtual std::tuple<int,rpc::int8> login(const std::string& opendid) {
	  std::cout << opendid << std::endl;
	  return std::make_tuple(0,1); 
	};
	virtual std::tuple<int, shuai::user> xixi(shuai::user&  u) {
	  return std::make_tuple(0,u); 
	};
	virtual std::tuple<int, std::vector<shuai::role> >lala(std::map<rpc::int32, shuai::role>&  m, std::vector<rpc::int32> &  ai, std::vector<shuai::role> &  ar) {
	  std::vector<shuai::role> roles;
	  return std::make_tuple(0,roles); 
	};
	virtual std::tuple<int, rpc::int8>ping(rpc::int8 i8) {
	  return std::make_tuple(0,i8); 
	};

};
int main(int argc,char** argv)
{
	rpc::EndPoint ep(std::string(":2619"));
	using rpcServiceType = rpc::RpcService<rpc::TcpTransport, rpc::BinaryProtocol, HelloMsgProxy>;
	rpc::RpcServer<rpcServiceType> server(rpc::NetEvent::getInstance(),ep);
	if (-1 == server.open()) {
		std::cerr << "service.open " << std::endl;
	}

	int ret=rpc::NetEvent::getInstance()->eventLoop();
	server.close();
	return 0;
}