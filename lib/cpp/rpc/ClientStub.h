
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

	virtual void invoke(const RpcMsg* m) {
		chan_->sendRequestMsg(m);
	}

	virtual void dispatch(const RpcMsg& m)=0;
protected:
	RpcChannel* chan_;
};