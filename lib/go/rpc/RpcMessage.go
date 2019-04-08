package rpc

import "time"

type RequestMsg struct {
	MsgId       uint16
	MsgSeqId    int64
	serviceName string
}

type ResponseMsg struct {
	MsgId       uint16
	MsgSeqId    int64
	serviceName string
}

type RpcMsg struct {
	ServiceName  string
	FunctionName string
	Time         time.Time
	RequestMsg   RequestMsg
	ResponseMsg  ResponseMsg
}
