package rpc

type IRpcServiecFactroy interface {
	CreateRpcServiec() interface{}
}

type RpcService struct {
	Connection
	proxyMap map[string]*ServiceProxy
}
