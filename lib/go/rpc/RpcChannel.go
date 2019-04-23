package rpc

import (
	"bytes"
	"net"
)

type RpcChannel struct {
	*connection
	transFactory ITransportFactory
	stubMap      map[string]*ServiceStub
}

func NewRpcChannel(event *NetEvent, ep endPoint, proto IProtocol, transFactory ITransportFactory) *RpcChannel {
	tcpConn, err := net.DialTCP("tcp", nil, ep.GetTcpAddr())
	if err != nil {
		return nil
	}

	conn := NewConnection(event, ep, tcpConn, proto, transFactory.NewTransport())
	rpcChan := &RpcChannel{connection: conn, transFactory: transFactory, stubMap: make(map[string]*ServiceStub)}
	rpcChan.connection.cb = rpcChan.handleInput
	return rpcChan
}

func (this *RpcChannel) handleInput(buff *bytes.Buffer) error {
	responseMsg, err := this.GetTransport().recvResponseMsg(buff)
	if err != nil {
	}

	stub, ok := this.stubMap[responseMsg.Header.ServiceName]
	if ok {
		stub.stubMsgCallBack(responseMsg)
	}
	return nil
}
