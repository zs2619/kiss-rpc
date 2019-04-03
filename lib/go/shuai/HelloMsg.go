package shuai

import "zs/rpc"

type OpServiceStub struct {
	stub         *rpc.ServiceStub
	testCallBack TestOpServiceStubCallBackType
}
type TestOpServiceStubCallBackType func(int32) int

func (this *OpServiceStub) invokeAsync(msgId uint16, p *rpc.IProtocol, funcName string) {
	msg := &rpc.RpcMsg{}
	this.stub.Invoke(msg)

}
func (this *OpServiceStub) Dispatch(msg *rpc.RpcMsg) bool {
	id := msg.ResponseMsg.MsgId
	switch id {
	case 1:
		proto := this.stub.GetProtocol()
		ret := proto.ReadInt32()
		cbRet := this.testCallBack(ret)
		if cbRet == -1 {
			return true
		}
	default:
	}

	return true
}
func (this *OpServiceStub) Test(i int, cb TestOpServiceStubCallBackType) bool {
	this.testCallBack = cb
	return true
}

func NewOpServiceStub(rpcChan *rpc.RpcChannel) *OpServiceStub {
	return &OpServiceStub{stub: rpc.NewServiceStub(&rpcChan.Connection)}
}

type opServiceProxyIF struct {
}