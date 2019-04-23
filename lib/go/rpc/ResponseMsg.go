package rpc

const ResponseMsg_strFingerprint = "ac24ea21771056228cb098ed462458e0"

type ResponseMsg struct {
	Header   RpcHeader `json:"header"`
	MsgSeqId int64     `json:"msgSeqId"`
	MsgId    uint16    `json:"msgId"`
	Buff     []int8    `json:"buff"`
}

func (this *ResponseMsg) GetFingerprint() string {
	return ResponseMsg_strFingerprint
}
func (this *ResponseMsg) Serialize(P__ IProtocol) {
	this.Header.Serialize(P__)

	P__.WriteInt64(this.MsgSeqId)

	P__.WriteUInt16(this.MsgId)

	P__.WriteUInt16(uint16(len(this.Buff)))
	for _, v := range this.Buff {
		P__.WriteInt8(v)
	}

}
func (this *ResponseMsg) DeSerialize(P__ IProtocol) bool {
	this.Header.DeSerialize(P__)

	this.MsgSeqId = P__.ReadInt64()

	this.MsgId = P__.ReadUInt16()

	_n_1_array := P__.ReadUInt16()
	for _i_1_ := 0; uint16(_i_1_) < _n_1_array; _i_1_++ {
		var tmp int8
		tmp = P__.ReadInt8()
		this.Buff = append(this.Buff, tmp)
	}

	return true
}
