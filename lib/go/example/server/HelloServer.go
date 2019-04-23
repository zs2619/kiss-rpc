package main

import (
	"kiss/rpc"
	"kiss/shuai"
	"reflect"
)

func NewShuaiOpServiceProxyFactory(rpcService *rpc.RpcService) rpc.IServiceProxy {
	return shuai.NewOpServiceProxy(rpcService, shuaiOpServiceProxy{})
}

type shuaiOpServiceProxy struct {
}

func (this *shuaiOpServiceProxy) Momo(i8 int8, i64 int64) error {

	return nil
}
func (this *shuaiOpServiceProxy) Test(i8 int8) (int8, error) {

	return 0, nil
}
func (this *shuaiOpServiceProxy) Xixi(u shuai.User) (int8, error) {

	return 0, nil
}
func (this *shuaiOpServiceProxy) Lala(m map[int32]shuai.Role, ai []int32, ar []shuai.Role) ([]shuai.Role, error) {

	return nil, nil
}
func (this *shuaiOpServiceProxy) Ping() (int8, error) {

	return 0, nil
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
