package rpc

const RequestMsg_strFingerprint = "e61c37380db853addbaafae6c4850b2e"

type RequestMsg struct {
	Header   RpcHeader `json:"header"`
	MsgSeqId int64     `json:"msgSeqId"`
	MsgId    int32     `json:"msgId"`
	Buff     []byte    `json:"buff"`
}

func (this *RequestMsg) GetFingerprint() string {
	return RequestMsg_strFingerprint
}
func (this *RequestMsg) Serialize(P__ IProtocol) {
	this.Header.Serialize(P__)

	P__.WriteInt64(this.MsgSeqId)

	P__.WriteInt32(this.MsgId)

	P__.WriteBinary(this.Buff)

}
func (this *RequestMsg) DeSerialize(P__ IProtocol) error {
	this.Header.DeSerialize(P__)

	this.MsgSeqId = P__.ReadInt64()

	this.MsgId = P__.ReadInt32()

	this.Buff = P__.ReadBinary()

	return nil
}
