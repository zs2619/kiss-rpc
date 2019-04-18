package main

import (
	"kiss/rpc"
	"kiss/shuai"
	"reflect"
)

type shuaiOpServiceProxyFactory struct {
}

func (this *shuaiOpServiceProxyFactory) NewServiceProxy(rpcService *rpc.RpcService) rpc.IServiceProxy {
	return &shuaiOpServiceProxy{OpServiceProxy: shuai.NewOpServiceProxy(rpcService)}
}

type shuaiOpServiceProxy struct {
	*shuai.OpServiceProxy
}

func (this *shuaiOpServiceProxy) Test(i int32) int32 {

	return 1
}

func main() {

	serviceProxyFactoryType := []reflect.Type{reflect.TypeOf((*shuaiOpServiceProxyFactory)(nil))}

	factory := rpc.RpcServiecFactroy{TransFactory: rpc.TcpTransportFactory{},
		ProtoFactory: rpc.BinaryProtocolFactory{}, ServiceProxyType: serviceProxyFactoryType}
	event := rpc.NewNetEvent()
	ep := rpc.NewEndPoint("0.0.0.0:2619")
	server := rpc.NewRpcServer(event, *ep, factory)
	ret := server.Open()
	if ret == -1 {
		return
	}
	event.EventLoop()
	server.Close()
}
