
#include "RpcMessage.h"
class  ClientStub{
public:
	virtual void invoke(const RpcMsg& m) {
	}

	virtual void dispatch(const RpcMsg& m){
	}
protected:
	RpcChannle chan;
};