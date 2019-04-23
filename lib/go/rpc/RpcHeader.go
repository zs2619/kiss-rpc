package rpc

const RpcHeader_strFingerprint = "28310e0dc8ad86ce46bf42a5037ee8d6"

type RpcHeader struct {
	Version     uint8  `json:"version"`
	MsgType     int16  `json:"msgType"`
	ServiceName string `json:"serviceName"`
}

func (this *RpcHeader) GetFingerprint() string {
	return RpcHeader_strFingerprint
}
func (this *RpcHeader) Serialize(P__ IProtocol) {
	P__.WriteUInt8(this.Version)

	P__.WriteInt16(this.MsgType)

	P__.WriteString(this.ServiceName)

}
func (this *RpcHeader) DeSerialize(P__ IProtocol) bool {
	this.Version = P__.ReadUInt8()

	this.MsgType = P__.ReadInt16()

	this.ServiceName = P__.ReadString()

	return true
}
