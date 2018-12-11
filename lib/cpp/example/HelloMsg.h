#include <functional>  
#include <type_traits>
#include "Rpc/ClientStub.h"
#include "Rpc/ServiceProxy.h"
#include "Rpc/Protocol.h"

class HelloMsgStub  :public ClientStub{
public: 
	HelloMsgStub() {}

	void test(int a, std::function<int(int)>  cb)
	{
		RpcMsg m;
		testCallBack=cb;
		invoke(m);
	}
	void testMsg(std::string s,std::function<int(const std::string&)> cb){
		RpcMsg m;
		testMsgCallBack=cb;
		invoke(m);
	}

	virtual void invoke(const RpcMsg& m) {
	}

	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg.msgId==1){
			int i=0;
			testCallBack(i);
		} else if (m.recvMsg.msgId==2){

			std::string s="shuai";
			testMsgCallBack(s);
		}
	}
	Protocol proc;
protected:
     std::function<void(int)> testCallBack;
     std::function<void(const std::string&)> testMsgCallBack;
};


class HelloMsgProxy: public ServiceProxy{

	virtual std::pair<int,int> test(int a)
	{
	}

	virtual std::pair<int,std::string> testMsg(std::string s){

	}

	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg.msgId==1){
			int a=1;
			auto result = test(a);

		} else if (m.recvMsg.msgId==2){

			std::string s="shuai";
			auto result = testMsg(s);
		}
	}
};