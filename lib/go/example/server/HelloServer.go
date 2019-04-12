package main

import (
	"reflect"
	"zs/rpc"
)

type opServiceProxy struct {
}

func (this *opServiceProxy) Test(i int32) int32 {

	return 1
}
func (this *opServiceProxy) Dispatch(msg *rpc.RpcMsg) bool {

	return true
}

func main() {

	serviceProxyType := []reflect.Type{reflect.TypeOf((*opServiceProxy)(nil))}

	factory := rpc.RpcServiecFactroy{TransFactory: rpc.TcpTransportFactory{}, ProtoFactory: rpc.BinaryProtocolFactory{}, ServiceProxyType: serviceProxyType}
	event := rpc.NewNetEvent()
	ep := rpc.NewEndPoint("127.0.0.1:2619")
	server := rpc.NewRpcServer(event, *ep, factory)
	ret := server.Open()
	if ret == -1 {
		return
	}
	event.EventLoop()
	server.Close()
}
