#include <functional>  
#include <type_traits>
#include "Rpc/ClientStub.h"
#include "Rpc/ServiceProxy.h"
#include "Rpc/Protocol.h"

class HelloMsgStub  :public ClientStub{
public: 
	HelloMsgStub() {}
	virtual ~HelloMsgStub() {}

	void test(int a, std::function<int(int)>  cb)
	{
		Protocol outWriter=getChannel()->getProtocol()->createProtoBuffer();
		outWriter.write(a);

		testCallBack=cb;
		invokeAsync(outWriter);
	}
	void testMsg(std::string& s,std::function<int(const std::string&)> cb){

		Protocol outWriter=getChannel()->getProtocol()->createProtoBuffer();
		outWriter.write(s);

		testMsgCallBack=cb;
		invokeAsync(outWriter);
	}

	virtual void invokeAsync( Protocol& p) {
		RpcMsg* msg=new RpcMsg;
		msg->sendMsg.buf=p.getBuffer();
		getChannel()->sendAsyncRpcMsg((const RpcMsg*) msg);
	}

	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg.msgId==1){

			Protocol inReader=getChannel()->getProtocol()->createProtoBuffer();
			inReader.setBuffer(m.recvMsg.buf);
			int i;
			inReader.read(i);

			int ret=testCallBack(i);

		} else if (m.recvMsg.msgId==2){

			Protocol inReader=getChannel()->getProtocol()->createProtoBuffer();
			inReader.setBuffer(m.recvMsg.buf);

			std::string s;
			inReader.read(s);

			int ret=testMsgCallBack(s);
		}
	}
protected:
     std::function<int(int)> testCallBack;
     std::function<int(const std::string&)> testMsgCallBack;
};


class HelloMsgProxyIF: public ServiceProxy{

	virtual std::pair<int,int> test(int a)=0;
	virtual std::pair<int,std::string> testMsg(std::string s)=0;


	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg.msgId==1){
			Protocol inReader=getChannel()->getProtocol()->createProtoBuffer();
			inReader.setBuffer(m.recvMsg.buf);
			int a=1;
			inReader.read(a);

			auto result = test(a);
			Protocol outWriter=getChannel()->getProtocol()->createProtoBuffer();
			invoke

		} else if (m.recvMsg.msgId==2){

			std::string s="shuai";
			auto result = testMsg(s);
		}
	}
};