package shuai
import (
	"kiss/rpc"
)

const role_strFingerprint="5ad2cb5c4c22f48680d17d2a084021b2"
type Role struct{
	SexType int16 `json:"sexType"`
	B bool `json:"b"`
	By byte `json:"by"`
	I8 int8 `json:"i8"`
	I16 int16 `json:"i16"`
	I32 int32 `json:"i32"`
	I64 int64 `json:"i64"`
	F float32 `json:"f"`
	Str string `json:"str"`
	Bin []byte `json:"bin"`
}
func (this *Role) GetFingerprint() string{
	return role_strFingerprint
}
func (this *Role) Serialize( P__ rpc.IProtocol){
	P__.WriteInt16(this.SexType)

	P__.WriteBool(this.B)

	P__.WriteByte(this.By)

	P__.WriteInt8(this.I8)

	P__.WriteInt16(this.I16)

	P__.WriteInt32(this.I32)

	P__.WriteInt64(this.I64)

	P__.WriteFloat(this.F)

	P__.WriteString(this.Str)

	P__.WriteBinary(this.Bin)

}
func (this *Role) DeSerialize( P__ rpc.IProtocol) error{
	this.SexType=P__.ReadInt16()

	this.B=P__.ReadBool()


	this.I8=P__.ReadInt8()

	this.I16=P__.ReadInt16()

	this.I32=P__.ReadInt32()

	this.I64=P__.ReadInt64()

	this.F=P__.ReadFloat()

	this.Str=P__.ReadString()

	this.Bin=P__.ReadBinary()

	return nil
}

