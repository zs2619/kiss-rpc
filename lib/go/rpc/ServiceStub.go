package rpc

import "time"

type ServiceStub struct {
	*RpcChannel
	maxMsgSeqId      int64
	msgQueue         map[int64]*RpcMsg
	RpcMsgDispatchCB func(msg *RpcMsg) error
}

func (this *ServiceStub) Invoke(msg *RpcMsg) int {
	msg.RequestMsg.MsgSeqId = this.maxMsgSeqId + 1
	msg.RequestMsg.Header.Version = 0
	msg.Time = time.Now().UTC()
	msg.RequestMsg.Header.MsgType = MPT_Bin
	msg.RequestMsg.Header.ServiceName = msg.ServiceName

	ret := this.GetTransport().sendRequestMsg(&msg.RequestMsg)
	if ret != nil {
		return -1
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
	rpcMsg.ResponseMsg=*respMsg
	this.RpcMsgDispatchCB(rpcMsg)
	delete(this.msgQueue, respMsg.MsgSeqId)
	return nil
}
