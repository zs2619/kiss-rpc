package rpc

type ServiceStub struct {
	*RpcChannel
	maxMsgSeqId      int64
	msgQueue         map[int64]*RpcMsg
	RpcMsgDispatchCB func(msg *RpcMsg) error
}

func (this *ServiceStub) Invoke(msg *RpcMsg) int {
	msg.RequestMsg.MsgSeqId = this.maxMsgSeqId + 1

	ret := this.GetTransport().sendRequestMsg(&msg.RequestMsg)
	if ret != nil {
	}
	this.msgQueue[msg.RequestMsg.MsgSeqId] = msg
	return 0
}

func NewServiceStub(rpcChan *RpcChannel) *ServiceStub {
	stub := &ServiceStub{RpcChannel: rpcChan, maxMsgSeqId: 0, msgQueue: make(map[int64]*RpcMsg)}
	return stub
}

func (this *ServiceStub) stubMsgCallBack(respMsg *ResponseMsg) error {
	rpcMsg, ok := this.msgQueue[respMsg.MsgSeqId]
	if !ok {
		return nil
	}
	this.RpcMsgDispatchCB(rpcMsg)
	delete(this.msgQueue, respMsg.MsgSeqId)
	return nil
}
