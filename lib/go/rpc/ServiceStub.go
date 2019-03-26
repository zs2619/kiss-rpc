package rpc

type ServiceStub struct {
	*Connection
	maxMsgSeqId int64
	msgQueue    map[int64]*RpcMsg
}

func (this *ServiceStub) Invoke(m *RpcMsg) int {

	return 0
}

func (this *ServiceStub) Dispatch(m *RpcMsg) bool {

	return true
}

func NewServiceStub(conn *Connection) *ServiceStub {
	stub := &ServiceStub{Connection: conn, maxMsgSeqId: 0, msgQueue: make(map[int64]*RpcMsg)}
	return stub
}
