package rpc

import (
	"bytes"
	"net"
)

type ServiceProxyFactory func (rpcService *RpcService) interface{} 

type IRpcServiecFactroy interface {
	NewRpcService(event *NetEvent, ep endPoint, tcpConn *net.TCPConn) *RpcService
}

type RpcServiecFactroy struct {
	TransFactory     ITransportFactory
	ProtoFactory     IProtocolFactory
	ServiceProxyFactory []ServiceProxyFactory
}

func (this RpcServiecFactroy) NewRpcService(event *NetEvent, ep endPoint, tcpConn *net.TCPConn) *RpcService {
	conn := NewConnection(event, ep, tcpConn, this.ProtoFactory.NewProtocol(), this.TransFactory.NewTransport())

	proxyMap := make(map[string]interface{})
	service := &RpcService{connection: conn}

	for _, factory := range this.ServiceProxyFactory {
		proxy:=factory(service)
		name:=proxy.(interface{ GetObjName() string }).GetObjName()
		proxyMap[name] = proxy
	}
	service.proxyMap = proxyMap

	conn.cb = service.handleInput
	return service

}

type RpcService struct {
	*connection
	proxyMap map[string]interface{}
}

func (this *RpcService) handleInput(buff *bytes.Buffer) error {
	requestMsg, err := this.GetTransport().recvRequestMsg(buff)
	if err != nil {
		return nil
	}
	proxy, ok := this.proxyMap[requestMsg.Header.ServiceName]
	if !ok {
		return nil
	}
	rpcMsg := &RpcMsg{}
	rpcMsg.RequestMsg=*requestMsg
	err=proxy.(interface{ Dispatch(msg *RpcMsg) error }).Dispatch(rpcMsg)
	return err
}
