
#include "EventHandler.h"
class  ClientStub:public EventHandler{
public:

	ClientStub() {}
	virtual ~ClientStub() {}

	void setChannel(RpcChannel* chan){
		chan_=chan;
	}
	const RpcChannel* getChannel(){
		return chan_;
	}

protected:
	RpcChannel* chan_;
};