
#include "RpcMessage.h"
#include "rpc/RpcChannel.h"
class  ClientStub{
public:

	ClientStub() {}
	virtual ~ClientStub() {}

	void setChannel(RpcChannel* chan){
		chan_=chan;
	}
	const RpcChannel* getChannel(){
		return chan_;
	}

	virtual void invokeAsync(const RpcMsg* m) {
		chan_->sendAsyncRpcMsg(m);
	}

	virtual void dispatch(const RpcMsg& m){
	}
protected:
	RpcChannel* chan_;
};