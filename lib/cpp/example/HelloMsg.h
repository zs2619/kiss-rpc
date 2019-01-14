
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
		std::unique_ptr<IProtocol> outWriter(getProtocol()->createProtoBuffer());
		
		outWriter->writeInt32(a);
		std::vector<uint8> h{1};
		outWriter->writeVec(h);
		testCallBack=cb;
		invokeAsync(outWriter.get());
	}

	void testMsg(std::string& s,std::function<int(const std::string&)> cb){

		std::unique_ptr<IProtocol> outWriter(getProtocol()->createProtoBuffer());
		outWriter->writeString(s);

		testMsgCallBack=cb;
		invokeAsync(outWriter.get());
	}

	void invokeAsync( const IProtocol* p) {
 	    std::shared_ptr<RpcMsg> msg = std::make_shared<RpcMsg>();
		msg->sendMsg_.buf=p->getBuffer();
		invoke(msg);
	}

	virtual void dispatch(std::shared_ptr<RpcMsg> msg){
		if (msg->recvMsg_.msgId==1){

			std::unique_ptr<IProtocol> inReader(getProtocol()->createProtoBuffer());
			inReader->setBuffer(msg->recvMsg_.buf);
			int i=0;
			inReader->readInt32(i);

			int ret=testCallBack(i);

		} else if (msg->recvMsg_.msgId==2){

			std::unique_ptr<IProtocol> inReader(getProtocol()->createProtoBuffer());
			inReader->setBuffer(msg->recvMsg_.buf);

			std::string s;
			inReader->readString(s);

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

	virtual void dispatch(std::shared_ptr<RpcMsg> msg){
		if (msg->recvMsg_.msgId==1){

			std::unique_ptr<IProtocol> inReader(getProtocol()->createProtoBuffer());
			inReader->setBuffer(msg->recvMsg_.buf);
			int a=1;
			inReader->readInt32(a);

			auto result = test(a);
			IProtocol* outWriter=getProtocol()->createProtoBuffer();

			invoke(msg);

		} else if (msg->recvMsg_.msgId==2){
			std::unique_ptr<IProtocol> inReader(getProtocol()->createProtoBuffer());
			inReader->setBuffer(msg->recvMsg_.buf);

			std::string str;
			inReader->readString(str);
			auto result = testMsg(str);

			invoke(msg);
		}
	}
};
#endif