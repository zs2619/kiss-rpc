package main

import (
	"fmt"
	"kiss/rpc"
	"kiss/shuai"

	"github.com/sirupsen/logrus"
)

func main() {
	event := rpc.NewNetEvent()
	rpcChan := rpc.NewRpcChannel(event, *rpc.NewEndPoint("127.0.0.1:2619"), new(rpc.BinaryProtocol), new(rpc.TcpTransportFactory))
	if rpcChan == nil {
		return
	}
	opServiceStub := shuai.NewOpServiceStub(rpcChan)

	event.ScheduleTimer(1000*5, func() error {
		logrus.Info("ScheduleTimer")
		return nil
	})
	opServiceStub.Login("shuai", func(i8 int8) error {
		fmt.Printf("%d", i8)
		return nil
	})
	event.EventLoop()
}
