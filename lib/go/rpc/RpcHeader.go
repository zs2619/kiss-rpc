package rpc

const RpcHeader_strFingerprint = "2bcc93ddec2a7ac9beece683a78b5561"

type RpcHeader struct {
	Version     int8   `json:"version"`
	MsgType     int16  `json:"msgType"`
	ServiceName string `json:"serviceName"`
}

func (this *RpcHeader) GetFingerprint() string {
	return RpcHeader_strFingerprint
}
func (this *RpcHeader) Serialize(P__ IProtocol) {
	P__.WriteInt8(this.Version)

	P__.WriteInt16(this.MsgType)

	P__.WriteString(this.ServiceName)

}
func (this *RpcHeader) DeSerialize(P__ IProtocol) error {
	this.Version = P__.ReadInt8()

	this.MsgType = P__.ReadInt16()

	this.ServiceName = P__.ReadString()

	return nil
}
