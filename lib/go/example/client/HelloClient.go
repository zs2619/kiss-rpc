package main

import (
	"kiss/rpc"
	"kiss/shuai"
	"fmt"
)

func main() {
	event := rpc.NewNetEvent()
	rpcChan := rpc.NewRpcChannel(event, *rpc.NewEndPoint("127.0.0.1:2619"), new(rpc.BinaryProtocol), new(rpc.TcpTransportFactory))
	if rpcChan == nil {
		return
	}
	opServiceStub := shuai.NewOpServiceStub(rpcChan)
	opServiceStub.Login("shuai", func(i8 int8) error {
		fmt.Printf("%d",i8)
		return nil
	})
	event.EventLoop()
}
