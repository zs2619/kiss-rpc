package rpc

type IRpcServiecFactroy interface {
	CreateRpcServiec() interface{}
}

type RpcService struct {
	connection
	proxyMap map[string]*ServiceProxy
}
