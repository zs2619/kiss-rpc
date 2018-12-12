
#include "RpcMessage.h"
#include "rpc/RpcChannel.h"
class  ClientStub{
public:
	virtual void invokeAsync(const RpcMsg& m) {
	}

	virtual void dispatch(const RpcMsg& m){
	}
protected:
	RpcChannel* chan;
};