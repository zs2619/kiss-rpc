package shuai

import (
	"kiss/rpc"
	"bytes"
)
type IOpServiceProxy interface {
	Login (openid  string) (int8,error)
	Xixi (u  User) (User,error)
	Lala (m  map[int32]Role,ai  []int32,ar  []Role) ([]Role,error)
	Ping (i8  int8) (int8,error)
}

const opService_strFingerprintProxy="878e3bfc527601565994f11bb9d347d0"
const opService_getObjNameProxy="shuai.opService";
type OpServiceProxy struct {
	*rpc.ServiceProxy
	strFingerprint string
	getObjName string
	cbObject IOpServiceProxy
}

func NewOpServiceProxy(rpcService *rpc.RpcService,obj IOpServiceProxy) *OpServiceProxy {
	return &OpServiceProxy{strFingerprint: opService_strFingerprintProxy, getObjName:opService_getObjNameProxy, ServiceProxy:rpc.NewServiceProxy(rpcService), cbObject:obj }
}
func (this *OpServiceProxy) GetFingerprint() string{
	return opService_strFingerprintProxy
}
func (this *OpServiceProxy) GetObjName() string{
	return this.getObjName
}
func (this *OpServiceProxy) Dispatch( msg *rpc.RpcMsg ) error {
	id := msg.RequestMsg.MsgId
	switch id {
	case 0:
		proto := this.GetProtocol().CreateProtoBuffer()
		proto.SetBuffer(bytes.NewBuffer(msg.RequestMsg.Buff))
		var openid string
		openid=proto.ReadString()

		result,err := this.cbObject.Login(openid)
		if err==nil{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.WriteInt8(result)
			msg.ResponseMsg.Buff=proto.GetBuffer().Bytes();
			this.Invoke(msg);
		}
		return nil
	case 1:
		proto := this.GetProtocol().CreateProtoBuffer()
		proto.SetBuffer(bytes.NewBuffer(msg.RequestMsg.Buff))
		var u User
		u.DeSerialize(proto)

		result,err := this.cbObject.Xixi(u)
		if err==nil{
			proto := this.GetProtocol().CreateProtoBuffer()
			result.Serialize(proto)
			msg.ResponseMsg.Buff=proto.GetBuffer().Bytes();
			this.Invoke(msg);
		}
		return nil
	case 2:
		proto := this.GetProtocol().CreateProtoBuffer()
		proto.SetBuffer(bytes.NewBuffer(msg.RequestMsg.Buff))
		var m map[int32]Role
		_n_0_map:=proto.ReadInt32()
		for _i_0_:=0; int32(_i_0_)<_n_0_map; _i_0_++ {
			var tmpKey int32
			tmpKey=proto.ReadInt32()
			var tmpValue Role
			tmpValue.DeSerialize(proto)
			m[tmpKey]=tmpValue
		}

		var ai []int32
		_n_1_array:=proto.ReadInt32()
		for _i_1_:=0; int32(_i_1_)<_n_1_array; _i_1_++ {
			var tmp int32
			tmp=proto.ReadInt32()
			ai= append(ai,tmp)
		}

		var ar []Role
		_n_2_array:=proto.ReadInt32()
		for _i_2_:=0; int32(_i_2_)<_n_2_array; _i_2_++ {
			var tmp Role
			tmp.DeSerialize(proto)
			ar= append(ar,tmp)
		}

		result,err := this.cbObject.Lala(m,ai,ar)
		if err==nil{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.WriteInt32(int32(len(result)))
			for _ ,v := range result {
				v.Serialize(proto)
			}
			msg.ResponseMsg.Buff=proto.GetBuffer().Bytes();
			this.Invoke(msg);
		}
		return nil
	case 3:
		proto := this.GetProtocol().CreateProtoBuffer()
		proto.SetBuffer(bytes.NewBuffer(msg.RequestMsg.Buff))
		var i8 int8
		i8=proto.ReadInt8()

		result,err := this.cbObject.Ping(i8)
		if err==nil{
			proto := this.GetProtocol().CreateProtoBuffer()
			proto.WriteInt8(result)
			msg.ResponseMsg.Buff=proto.GetBuffer().Bytes();
			this.Invoke(msg);
		}
		return nil
	default:
	return nil
	}//switch 
}//dispatch func
