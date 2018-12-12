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
		Protocol outWriter=chan->getProtocol()->createProtoBuffer();
		outWriter.write(a);

		testCallBack=cb;
		invokeAsync(&outWriter);
	}
	void testMsg(std::string s,std::function<int(const std::string&)> cb){
		RpcMsg m;
		testMsgCallBack=cb;
		invokeAsync(m);
	}

	virtual void invokeAsync(const Protocol& p) {
		RpcMsg* msg=new RpcMsg;
		m.sendMsg.buf=p.getBuffer()
		chan->sendAsyncRpcMsg(&msg);
	}

	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg.msgId==1){

			Protocol inReader=chan->getProtocol()->createProtoBuffer();
			inReader->setBuffer(m.recvMsg.buf);
			int i;
			inWriter.read(i);

			int ret=testCallBack(i);

		} else if (m.recvMsg.msgId==2){

			Protocol inReader=chan->getProtocol()->createProtoBuffer();
			inReader->setBuffer(m.recvMsg.buf);

			std::string s;
			inReader.read(s);

			int ret=testMsgCallBack(s);
		}
	}
protected:
     std::function<void(int)> testCallBack;
     std::function<void(const std::string&)> testMsgCallBack;
};


class HelloMsgProxyIF: public ServiceProxy{

	virtual std::pair<int,int> test(int a)=0;
	virtual std::pair<int,std::string> testMsg(std::string s)=0;


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