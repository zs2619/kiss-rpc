package shuai

import (
	"kiss/rpc"
	"bytes"
)
const opService_strFingerprintStub="878e3bfc527601565994f11bb9d347d0"
const opService_getObjNameStub="shuai.opService";
type OpServiceStub struct {
	*rpc.ServiceStub
	strFingerprint string
	getObjName string

	loginCallBack LoginCallBackType
	xixiCallBack XixiCallBackType
	lalaCallBack LalaCallBackType
	pingCallBack PingCallBackType
}

func NewOpServiceStub(rpcChan *rpc.RpcChannel) *OpServiceStub{
	stub := &OpServiceStub{strFingerprint: opService_strFingerprintStub, getObjName: opService_getObjNameStub, ServiceStub: rpc.NewServiceStub(rpcChan)}
	rpcChan.AddStubMap(stub.getObjName,stub.ServiceStub)
	stub.ServiceStub.RpcMsgDispatchCB = stub.Dispatch
	return stub
}
func (this *OpServiceStub) invokeAsync(msgId int32, p rpc.IProtocol, funcName string) {
	msg := &rpc.RpcMsg{}
	msg.FunctionName = funcName
	msg.ServiceName = this.GetObjName()
	msg.RequestMsg.MsgId = msgId
	msg.RequestMsg.Buff = p.GetBuffer().Bytes()
	this.Invoke(msg)
}
type LoginCallBackType func (int8) error
type XixiCallBackType func (User) error
type LalaCallBackType func ([]Role) error
type PingCallBackType func (int8) error

func (this *OpServiceStub) GetFingerprint() string{
	return this.strFingerprint
}
func (this *OpServiceStub) GetObjName() string{
	return this.getObjName
}
func (this *OpServiceStub) Login (openid  string,cb LoginCallBackType) error {
	proto := this.GetProtocol().CreateProtoBuffer()
	proto.WriteString(openid)

	this.invokeAsync(0, proto, "login")
	this.loginCallBack = cb
	return nil
}
func (this *OpServiceStub) Xixi (u  User,cb XixiCallBackType) error {
	proto := this.GetProtocol().CreateProtoBuffer()
	u.Serialize(proto)

	this.invokeAsync(1, proto, "xixi")
	this.xixiCallBack = cb
	return nil
}
func (this *OpServiceStub) Lala (m  map[int32]Role,ai  []int32,ar  []Role,cb LalaCallBackType) error {
	proto := this.GetProtocol().CreateProtoBuffer()
	proto.WriteInt32(int32(len(m)))
	for k ,v := range m {
		proto.WriteInt32(k)
		v.Serialize(proto)
	}

	proto.WriteInt32(int32(len(ai)))
	for _ ,v := range ai {
		proto.WriteInt32(v)
	}

	proto.WriteInt32(int32(len(ar)))
	for _ ,v := range ar {
		v.Serialize(proto)
	}

	this.invokeAsync(2, proto, "lala")
	this.lalaCallBack = cb
	return nil
}
func (this *OpServiceStub) Ping (i8  int8,cb PingCallBackType) error {
	proto := this.GetProtocol().CreateProtoBuffer()
	proto.WriteInt8(i8)

	this.invokeAsync(3, proto, "ping")
	this.pingCallBack = cb
	return nil
}
func (this *OpServiceStub) Dispatch(msg *rpc.RpcMsg) error {
	 id:=msg.ResponseMsg.MsgId
	switch id {
		case 0:
		{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.SetBuffer(bytes.NewBuffer(msg.ResponseMsg.Buff))
			var ret int8
			ret=proto.ReadInt8()
			cbRet := this.loginCallBack(ret)
			return cbRet
		}
		case 1:
		{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.SetBuffer(bytes.NewBuffer(msg.ResponseMsg.Buff))
			var ret User
			ret.DeSerialize(proto)
			cbRet := this.xixiCallBack(ret)
			return cbRet
		}
		case 2:
		{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.SetBuffer(bytes.NewBuffer(msg.ResponseMsg.Buff))
			var ret []Role
			_n_2_array:=proto.ReadInt32()
			for _i_2_:=0; int32(_i_2_)<_n_2_array; _i_2_++ {
				var tmp Role
				tmp.DeSerialize(proto)
				ret= append(ret,tmp)
			}
			cbRet := this.lalaCallBack(ret)
			return cbRet
		}
		case 3:
		{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.SetBuffer(bytes.NewBuffer(msg.ResponseMsg.Buff))
			var ret int8
			ret=proto.ReadInt8()
			cbRet := this.pingCallBack(ret)
			return cbRet
		}
		default:
		{
			return nil
		}
	}
}

