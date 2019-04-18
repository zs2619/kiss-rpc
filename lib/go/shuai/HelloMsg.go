package shuai

import "kiss/rpc"

type OpServiceStub struct {
	*rpc.ServiceStub
	testCallBack   TestOpServiceStubCallBackType
	strFingerprint string
	getObjName     string
}
type TestOpServiceStubCallBackType func(int32) int32

func (this *OpServiceStub) invokeAsync(msgId uint16, p rpc.IProtocol, funcName string) {
	msg := &rpc.RpcMsg{}
	this.Invoke(msg)
}
func (this *OpServiceStub) Dispatch(msg *rpc.RpcMsg) bool {
	id := msg.ResponseMsg.MsgId
	switch id {
	case 1:
		proto := this.GetProtocol()
		ret := proto.ReadInt32()
		cbRet := this.testCallBack(ret)
		if cbRet == -1 {
			return true
		}
	default:
	}

	return true
}
func (this *OpServiceStub) Test(i int32, cb TestOpServiceStubCallBackType) bool {
	proto := this.GetProtocol().CreateProtoBuffer()
	this.invokeAsync(1, proto, "test")
	this.testCallBack = cb
	return true
}

func NewOpServiceStub(rpcChan *rpc.RpcChannel) *OpServiceStub {
	stub := &OpServiceStub{strFingerprint: "shuai", getObjName: "OpService", ServiceStub: rpc.NewServiceStub(rpcChan)}
	stub.ServiceStub.RpcMsgDispatchCB = stub.Dispatch
	return stub
}
func (this *OpServiceStub) GetObjName() string {
	return this.getObjName
}
func (this *OpServiceStub) GetStrFingerprint() string {
	return this.strFingerprint
}

type OpServiceProxyIF interface {
	Test(i int32) int32
}

type OpServiceProxy struct {
	strFingerprint string
	getObjName     string
	*rpc.RpcService
}

func NewOpServiceProxy(rpcService *rpc.RpcService) *OpServiceProxy {
	return &OpServiceProxy{strFingerprint: "shuai", getObjName: "OpService", RpcService: rpcService}
}

func (this *OpServiceProxy) Dispatch(msg *rpc.RpcMsg) bool {

	return true
}

func (this *OpServiceProxy) GetObjName() string {
	return this.getObjName
}
func (this *OpServiceProxy) GetStrFingerprint() string {
	return this.strFingerprint
}
