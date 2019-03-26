package main

import (
	"zs/rpc"
)

func main() {
	event := rpc.NewNetEvent()
	ep := rpc.NewEndPoint("127.0.0.1:2619")
	server := rpc.NewRpcServer(*ep, event)
	ret := server.Open()
	if ret == -1 {
		return
	}
	event.EventLoop()
	server.Close()
}
