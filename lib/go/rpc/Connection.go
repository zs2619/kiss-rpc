package rpc

import (
	"bytes"
	"net"
	"sync"
)

type connection struct {
	proto    IProtocol
	trans    ITransport
	conn     *net.TCPConn
	sendCh   chan *bytes.Buffer
	IsValid  bool
	exitSync sync.WaitGroup
	event    *NetEvent
	cb       commonMsgCB
}

func NewConnection(event *NetEvent, ep endPoint, tcpConn *net.TCPConn, proto IProtocol, trans ITransport) *connection {
	conn := &connection{event: event, proto: proto, trans: trans, conn: tcpConn, sendCh: make(chan *bytes.Buffer, 1024), IsValid: true}
	trans.start(conn)
	return conn
}

func (this *connection) GetProtocol() IProtocol {
	return this.proto
}
func (this *connection) GetTransport() ITransport {
	return this.trans
}
