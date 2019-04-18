package rpc

type IServiceProxyFactory interface {
	NewServiceProxy(rpcService *RpcService) IServiceProxy
}
type IServiceProxy interface {
	Dispatch(msg *RpcMsg) bool
	GetObjName() string
}

type ServiceProxy struct {
	*connection
}

func (this *ServiceProxy) Invoke(m *RpcMsg) int {

	return 0
}

func NewServiceProxy(rpcService *RpcService) *ServiceProxy {
	proxy := &ServiceProxy{connection: rpcService.connection}
	return proxy
}
