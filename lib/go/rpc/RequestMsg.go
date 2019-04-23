package rpc

const RequestMsg_strFingerprint = "2017ec81ad4621da5e3b8bd32bc985fe"

type RequestMsg struct {
	Header   RpcHeader `json:"header"`
	MsgSeqId int64     `json:"msgSeqId"`
	MsgId    uint16    `json:"msgId"`
	Buff     []int8    `json:"buff"`
}

func (this *RequestMsg) GetFingerprint() string {
	return RequestMsg_strFingerprint
}
func (this *RequestMsg) Serialize(P__ IProtocol) {
	this.Header.Serialize(P__)

	P__.WriteInt64(this.MsgSeqId)

	P__.WriteUInt16(this.MsgId)

	P__.WriteUInt16(uint16(len(this.Buff)))
	for _, v := range this.Buff {
		P__.WriteInt8(v)
	}

}
func (this *RequestMsg) DeSerialize(P__ IProtocol) bool {
	this.Header.DeSerialize(P__)

	this.MsgSeqId = P__.ReadInt64()

	this.MsgId = P__.ReadUInt16()

	_n_0_array := P__.ReadUInt16()
	for _i_0_ := 0; uint16(_i_0_) < _n_0_array; _i_0_++ {
		var tmp int8
		tmp = P__.ReadInt8()
		this.Buff = append(this.Buff, tmp)
	}

	return true
}
