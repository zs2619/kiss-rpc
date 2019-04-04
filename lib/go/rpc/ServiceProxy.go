package rpc

type ServiceProxy struct {
	*connection
}

func (this *ServiceProxy) Invoke(m *RpcMsg) int {

	return 0
}

func NewServiceProxy(conn *connection) *ServiceProxy {
	proxy := &ServiceProxy{connection: conn}
	return proxy
}
