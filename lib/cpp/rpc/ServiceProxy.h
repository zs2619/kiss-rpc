
#include "RpcMessage.h"
#include "rpc/RpcService.h"

class ServiceProxy:public EventHandler{
public:

	const RpcService* getService(){
		return service_;
	}


protected:
	RpcService*  service_;
};