
#include "RpcMessage.h"
#include "rpc/RpcService.h"
class ServiceProxy{
public:
	virtual void dispatch(const RpcMsg& m)=0;
	virtual void invoke(const RpcMsg& m)=0;

	const RpcService* getService(){
		return service_;
	}
	virtual void invoke(const RpcMsg* m) {
		service_->sendResponseMsg(m);
	}


protected:
	RpcService*  service_;
};