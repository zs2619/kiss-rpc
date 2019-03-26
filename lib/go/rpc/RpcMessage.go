package rpc

import "time"

type RequestMsg struct {
	MsgId uint16
}

type ResponseMsg struct {
	MsgId uint16
}

type RpcMsg struct {
	ServiceName  string
	FunctionName string
	Time         time.Time
	RequestMsg   RequestMsg
	ResponseMsg  ResponseMsg
}
