package rpc

import (
	"net"
)

type RpcServer struct {
	ep                endPoint
	netEvent          *NetEvent
	listener          *net.TCPListener
	RpcServiecFactroy IRpcServiecFactroy
	tcpAddr           *net.TCPAddr
}

func (this *RpcServer) Open() int {
	var err error
	this.tcpAddr, err = net.ResolveTCPAddr("tcp", this.ep.GetHostName())
	if err != nil {
		return -1
	}

	ln, err := net.ListenTCP("tcp", this.tcpAddr)
	if err != nil {
		return -1
	}

	this.listener = ln
	go this.accept()
	return 0
}
func (this *RpcServer) Close() {

}

func (this *RpcServer) accept() {

	for {
		tcpConn, err := this.listener.AcceptTCP()
		if err != nil {
			return
		}
		this.RpcServiecFactroy.NewRpcService(this.netEvent, this.ep, tcpConn)
	}
}

func NewRpcServer(ep endPoint, event *NetEvent) *RpcServer {

	return &RpcServer{
		ep:       ep,
		netEvent: event,
	}
}
