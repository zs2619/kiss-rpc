package rpc

type ServiceStub struct {
	*RpcChannel
	maxMsgSeqId int64
	msgQueue    map[int64]*RpcMsg
}

func (this *ServiceStub) Invoke(m *RpcMsg) int {

	return 0
}

func NewServiceStub(rpcChan *RpcChannel) *ServiceStub {
	stub := &ServiceStub{RpcChannel: rpcChan, maxMsgSeqId: 0, msgQueue: make(map[int64]*RpcMsg)}
	return stub
}
