package main

import (
	"kiss/rpc"
	"kiss/shuai"
)

func NewShuaiOpServiceProxyFactory(rpcService *rpc.RpcService) interface{} {
	return shuai.NewOpServiceProxy(rpcService, &shuaiOpServiceProxy{})
}

type shuaiOpServiceProxy struct {
}

func (this *shuaiOpServiceProxy) Login(openid string) (int8, error) {

	return 1, nil
}
func (this *shuaiOpServiceProxy) Ping(i8 int8) (int8, error) {

	return 0, nil
}
func (this *shuaiOpServiceProxy) Xixi(u shuai.User) (shuai.User, error) {
	return u, nil
}
func (this *shuaiOpServiceProxy) Lala(m map[int32]shuai.Role, ai []int32, ar []shuai.Role) ([]shuai.Role, error) {
	return ar, nil
}

func main() {
	serviceProxyFactory := []rpc.ServiceProxyFactory{NewShuaiOpServiceProxyFactory}

	factory := rpc.RpcServiecFactroy{TransFactory: rpc.TcpTransportFactory{},
		ProtoFactory: rpc.BinaryProtocolFactory{}, ServiceProxyFactory: serviceProxyFactory}

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
