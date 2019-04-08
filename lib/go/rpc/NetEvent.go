package rpc

import (
	"bytes"

	"github.com/sirupsen/logrus"
)

type commonMsgCB func(buff *bytes.Buffer) error
type commonMsg struct {
	cb      commonMsgCB
	msgType int8
	data    *bytes.Buffer
}
type NetEvent struct {
	queue chan *commonMsg
}

func (this *NetEvent) EventLoop() int {

	for cmsg := range this.queue {
		if cmsg == nil {
			logrus.Info("Main MsgLoop Quit")
			return -1
		} else {
			cmsg.cb(cmsg.data)
		}
	}
	return 0
}

var G_NetEvent *NetEvent

func NewNetEvent() *NetEvent {
	if G_NetEvent != nil {
		return G_NetEvent
	}
	G_NetEvent = &NetEvent{queue: make(chan *commonMsg, 1024*1024)}
	return G_NetEvent
}

func NewCommonMsg(cb commonMsgCB, msgType int8, data *bytes.Buffer) *commonMsg {
	return &commonMsg{cb: cb, msgType: msgType, data: data}
}
