package rpc

import (
	"net"
)

type RpcServer struct {
	ep                endPoint
	netEvent          *NetEvent
	listener          *net.TCPListener
	rpcServiecFactroy IRpcServiecFactroy
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
	this.listener.Close()
}

func (this *RpcServer) accept() {
	for {
		tcpConn, err := this.listener.AcceptTCP()
		if err != nil {
			return
		}
		this.rpcServiecFactroy.NewRpcService(this.netEvent, this.ep, tcpConn)
	}
}

func NewRpcServer(event *NetEvent, ep endPoint, factroy RpcServiecFactroy) *RpcServer {
	return &RpcServer{
		rpcServiecFactroy: factroy,
		ep:                ep,
		netEvent:          event,
	}
}
