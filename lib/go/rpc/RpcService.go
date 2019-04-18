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
	proxyMap := make(map[string]IServiceProxy)

	service := &RpcService{connection: conn}

	for _, t := range this.ServiceProxyType {
		spValue := reflect.New(t)
		vv := spValue.Elem().Interface().(IServiceProxyFactory)
		proxy := vv.NewServiceProxy(service)
		proxyMap[proxy.GetObjName()] = proxy
	}
	service.proxyMap = proxyMap
	conn.cb = service.handleInput
	return service

}

type RpcService struct {
	*connection
	proxyMap map[string]IServiceProxy
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
	proxy.Dispatch(rpcMsg)

	return nil
}
