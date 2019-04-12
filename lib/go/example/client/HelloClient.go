package main

import (
	"zs/rpc"
	"zs/shuai"
)

func main() {
	event := rpc.NewNetEvent()
	rpcChan := rpc.NewRpcChannel(event, *rpc.NewEndPoint("127.0.0.1:2619"), new(rpc.BinaryProtocol), new(rpc.TcpTransportFactory))
	if rpcChan == nil {
		return
	}
	opServiceStub := shuai.NewOpServiceStub(rpcChan)
	opServiceStub.Test(1, func(i int32) int32 {
		return 0
	})
	event.EventLoop()
}
