package main

import (
	"zs/rpc"
)

type opServiceProxy struct {
}

func (this *opServiceProxy) Test(i int32) int32 {

}
func (this *opServiceProxy) Dispatch(msg *rpc.RpcMsg) bool {

}

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
