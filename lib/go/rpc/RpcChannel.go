package rpc

type RpcChannel struct {
	*connection
	proxyMap map[string]*ServiceStub
}

func (this *RpcChannel) Open() {

}
func (this *RpcChannel) Close() {

}

func NewRpcChannel(event *NetEvent, ep endPoint, proto IProtocol, trans ITransport) *RpcChannel {
	rpcChan := &RpcChannel{connection: NewConnection(event, ep, proto, trans)}
	return rpcChan
}
