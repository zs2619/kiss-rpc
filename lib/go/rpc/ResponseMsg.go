package rpc

const ResponseMsg_strFingerprint = "d44e3caf8903d1f381a22194a181c1cc"

type ResponseMsg struct {
	Header   RpcHeader `json:"header"`
	MsgSeqId int64     `json:"msgSeqId"`
	MsgId    int32     `json:"msgId"`
	Buff     []byte    `json:"buff"`
}

func (this *ResponseMsg) GetFingerprint() string {
	return ResponseMsg_strFingerprint
}
func (this *ResponseMsg) Serialize(P__ IProtocol) {
	this.Header.Serialize(P__)

	P__.WriteInt64(this.MsgSeqId)

	P__.WriteInt32(this.MsgId)

	P__.WriteBinary(this.Buff)

}
func (this *ResponseMsg) DeSerialize(P__ IProtocol) error {
	this.Header.DeSerialize(P__)

	this.MsgSeqId = P__.ReadInt64()

	this.MsgId = P__.ReadInt32()

	this.Buff = P__.ReadBinary()

	return nil
}
