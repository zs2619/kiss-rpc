
#include "RpcMessage.h"
class ServiceProxy{
public:
	virtual void dispatch(const RpcMsg& m){
	}
	virtual void invoke(const RpcMsg& m) {
	}
};