package rpc

type RpcChannel struct {
	Connection
	proxyMap map[string]*ServiceStub
}

func (this *RpcChannel) Open() {

}
func (this *RpcChannel) Close() {

}

func NewRpcChannel(event *NetEvent, ep endPoint) *RpcChannel {

	return nil
}
