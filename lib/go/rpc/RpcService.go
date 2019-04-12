package rpc

import (
	"bytes"
	"net"
	"reflect"
)

type IRpcServiecFactroy interface {
	NewRpcService(event *NetEvent, ep endPoint, tcpConn *net.TCPConn) *RpcService
}
type RpcServiecFactroy struct {
	TransFactory     ITransportFactory
	ProtoFactory     IProtocolFactory
	ServiceProxyType []reflect.Type
}

func (this RpcServiecFactroy) NewRpcService(event *NetEvent, ep endPoint, tcpConn *net.TCPConn) *RpcService {
	conn := NewConnection(event, ep, tcpConn, this.ProtoFactory.NewProtocol(), this.TransFactory.NewTransport())
	proxyMap := make(map[string]*ServiceProxy)

	return &RpcService{connection: conn, proxyMap: proxyMap}
}

type RpcService struct {
	*connection
	proxyMap map[string]*ServiceProxy
}

func (this *RpcService) handleInput(buff *bytes.Buffer) error {
	requestMsg, err := this.GetTransport().recvRequestMsg(buff)
	if err != nil {
		return nil
	}

	proxy, ok := this.proxyMap[requestMsg.serviceName]
	if !ok {
		return nil
	}
	rpcMsg := &RpcMsg{}
	proxy.RpcMsgDispatchCB(rpcMsg)

	return nil
}
