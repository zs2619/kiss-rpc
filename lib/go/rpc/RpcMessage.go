package rpc

import (
	"time"
)

type RpcMsg struct {
	ServiceName  string
	FunctionName string
	Time         time.Time
	RequestMsg   RequestMsg
	ResponseMsg  ResponseMsg
}
