package rpc
const (
	MPT_Bin =iota 
	MPT_Json
)
var (
	MsgProtocolTypeS2I= map[string] int16 {
	"MPT_Bin" : MPT_Bin,
	"MPT_Json" : MPT_Json,
	}
	MsgProtocolTypeI2S= map[int16] string {
	MPT_Bin : "MPT_Bin",
	MPT_Json : "MPT_Json",
	}
)
