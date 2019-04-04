package rpc

import (
	"bytes"

	"github.com/sirupsen/logrus"
)

type CommonMsg struct {
	Conn *connection
	Msg  *bytes.Buffer
}
type NetEvent struct {
	queue chan *CommonMsg
}

func (this *NetEvent) EventLoop() int {

	for msg := range this.queue {
		if msg == nil {
			logrus.Info("Main MsgLoop Quit")
			return -1
		}
	}
	return 0
}

var G_NetEvent *NetEvent

func NewNetEvent() *NetEvent {
	if G_NetEvent != nil {
		return G_NetEvent
	}
	G_NetEvent = &NetEvent{}
	return G_NetEvent
}
