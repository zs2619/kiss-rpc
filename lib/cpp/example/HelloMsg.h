
#ifndef __RPC_HELLOMSG_H__
#define __RPC_HELLOMSG_H__

#include <functional>  
#include <type_traits>
#include "rpc/ClientStub.h"
#include "rpc/ServiceProxy.h"
#include "rpc/Protocol.h"

class HelloMsgStub  : public rpc::ClientStub{
public: 
	HelloMsgStub() {}
	virtual ~HelloMsgStub() {}

	void test(int a, std::function<int(int)>  cb) {
		Protocol outWriter=getProtocol()->createProtoBuffer();
		outWriter.write(a);

		testCallBack=cb;
		invokeAsync(outWriter);
	}

	void testMsg(std::string& s,std::function<int(const std::string&)> cb){

		Protocol outWriter=getProtocol()->createProtoBuffer();
		outWriter.write(s);

		testMsgCallBack=cb;
		invokeAsync(outWriter);
	}

	void invokeAsync( Protocol& p) {
		RpcMsg* msg=new RpcMsg;
		msg->sendMsg_.buf=p.getBuffer();
		invoke((const RpcMsg*) msg);
	}

	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg_.msgId==1){

			Protocol inReader=getProtocol()->createProtoBuffer();
			inReader.setBuffer(m.recvMsg_.buf);
			int i;
			inReader.read(i);

			int ret=testCallBack(i);

		} else if (m.recvMsg_.msgId==2){

			Protocol inReader=getProtocol()->createProtoBuffer();
			inReader.setBuffer(m.recvMsg_.buf);

			std::string s;
			inReader.read(s);

			int ret=testMsgCallBack(s);
		}
	}
protected:
     std::function<int(int)> testCallBack;
     std::function<int(const std::string&)> testMsgCallBack;
};


class HelloMsgProxyIF: public rpc::ServiceProxy{

	virtual std::pair<int,int> test(int a)=0;
	virtual std::pair<int,std::string> testMsg(std::string s)=0;

	virtual void dispatch(const RpcMsg& m){
		if (m.recvMsg_.msgId==1){
			Protocol inReader=getProtocol()->createProtoBuffer();
			inReader.setBuffer(m.recvMsg_.buf);
			int a=1;
			inReader.read(a);

			auto result = test(a);
			Protocol outWriter=getProtocol()->createProtoBuffer();

		} else if (m.recvMsg_.msgId==2){

			std::string s="shuai";
			auto result = testMsg(s);
		}
	}
};
#endif