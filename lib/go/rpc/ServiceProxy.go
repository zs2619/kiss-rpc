package rpc

type ServiceProxy struct {
	*connection
	RpcMsgDispatchCB func(msg *RpcMsg) bool
}

func (this *ServiceProxy) Invoke(m *RpcMsg) int {

	return 0
}

func NewServiceProxy(conn *connection) *ServiceProxy {
	proxy := &ServiceProxy{connection: conn}
	return proxy
}
