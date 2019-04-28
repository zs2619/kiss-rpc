package shuai
import (
	"kiss/rpc"
)

const user_strFingerprint="58d411b316d3ad09ca4c27108e0ca7d7"
type User struct{
	RoleMap map[int32]Role `json:"roleMap"`
	Items []int32 `json:"items"`
	Roles []Role `json:"roles"`
}
func (this *User) GetFingerprint() string{
	return user_strFingerprint
}
func (this *User) Serialize( P__ rpc.IProtocol){
	P__.WriteInt32(int32(len(this.RoleMap)))
	for k ,v := range this.RoleMap {
		P__.WriteInt32(k)
		v.Serialize(P__)
	}

	P__.WriteInt32(int32(len(this.Items)))
	for _ ,v := range this.Items {
		P__.WriteInt32(v)
	}

	P__.WriteInt32(int32(len(this.Roles)))
	for _ ,v := range this.Roles {
		v.Serialize(P__)
	}

}
func (this *User) DeSerialize( P__ rpc.IProtocol) error{
	_n_0_map:=P__.ReadInt32()
	for _i_0_:=0; int32(_i_0_)<_n_0_map; _i_0_++ {
		var tmpKey int32
		tmpKey=P__.ReadInt32()
		var tmpValue Role
		tmpValue.DeSerialize(P__)
		this.RoleMap[tmpKey]=tmpValue
	}

	_n_0_array:=P__.ReadInt32()
	for _i_0_:=0; int32(_i_0_)<_n_0_array; _i_0_++ {
		var tmp int32
		tmp=P__.ReadInt32()
		this.Items= append(this.Items,tmp)
	}

	_n_1_array:=P__.ReadInt32()
	for _i_1_:=0; int32(_i_1_)<_n_1_array; _i_1_++ {
		var tmp Role
		tmp.DeSerialize(P__)
		this.Roles= append(this.Roles,tmp)
	}

	return nil
}

