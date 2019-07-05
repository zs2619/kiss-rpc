
#ifndef __RPC_RPCChANNEL_H__
#define __RPC_RPCCHANNEL_H__

#include <string>
#include "rpc/EndPoint.h"
#include "rpc/Connection.h"
#include "rpc/NetEvent.h"
#include "rpc/EventHandler.h"
#include "rpc/RpcMessage.h"
#include "rpc/ServiceStub.h"

namespace rpc {
	
template<typename T,typename P>
class RpcChannel:public Connection
{
public:
	RpcChannel(NetEvent*  event,EndPoint ep):Connection(event,ep,new T(this),new P()){
		int  ret= connect();
		RpcAssert (-1 != ret);
	}

	~RpcChannel(){
	}

	template < typename E >
	E* createStub(){
		if (stubMap_.find(E::getObjName)!=stubMap_.end()) {
			return (E*)stubMap_[E::getObjName];
		}
		E* eh=new E(this);
		stubMap_[E::getObjName]=eh;
		return eh;
	}

	int close(){
	}

	virtual int handleInput(struct evbuffer* buff){
		std::vector<ResponseMsg> respMsgList;
		int ret=getTransport()->recvResponseMsg(buff,respMsgList);
		if (-1==ret){
			return 0;
		}
		for ( auto& it:respMsgList){
			auto stub = stubMap_.find(it.header.serviceName);
			if (stub==stubMap_.end()) {
				std::cout<<it.header.serviceName<<" error "<<std::endl;
				continue;
			}
			int ret=stub->second->stubMsgCallBack(it);
		}
		return 0;
	}


	virtual int handleConnction(struct bufferevent* bev){
		getTransport()->setBufferEvent(bev);
		isValid_=true;
		return 0;
	};

	virtual int handleClose(){
		std::cout<<"handleClose"<<std::endl;
		getTransport()->close();
		isValid_=false;
		return 0;
	}

private:

	static void connect_cb(struct bufferevent *bev, short events, void *ctx){
		RpcChannel* channel=(RpcChannel*)ctx;
		if (events & BEV_EVENT_EOF||
			events & BEV_EVENT_ERROR||
			events & BEV_EVENT_TIMEOUT) {
			channel->handleClose();
		}else if (events & BEV_EVENT_CONNECTED) {
			channel->handleConnction(bev);
		}
	}

	int connect(){
		struct event_base* base = event_->getInstance()->getEventBase();
		bufferevent* bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
		RpcAssert (bev != nullptr);

		bufferevent_setcb(bev, NULL , NULL , connect_cb, this);
		bufferevent_enable(bev, EV_WRITE);
		if (0!=bufferevent_socket_connect(bev,(const sockaddr*)ep_.getAddrIn(),sizeof(struct sockaddr_in))){
			return -1;
		}
		return 0;
	}

	std::map<std::string,ServiceStub*> stubMap_;
	bool isValid_=false;
};
}
#endif