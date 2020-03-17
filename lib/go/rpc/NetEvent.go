package rpc

import (
	"bytes"
	"sync"
	"sync/atomic"
	"time"

	"github.com/sirupsen/logrus"
)

type commonMsgCB func(buff *bytes.Buffer) error
type commonMsg struct {
	cb      commonMsgCB
	msgType int8
	data    *bytes.Buffer
}
type NetEvent struct {
	queue        chan *commonMsg
	timeOutMap   sync.Map
	timeOutIDGen int64
}

type TimeOut struct {
	msInterval time.Duration
	cb         func() error
	quit       chan bool
	ne         *NetEvent
}

func (this *TimeOut) Run() error {
	ticker := time.NewTicker(this.msInterval)
	for {
		select {
		case <-this.quit:
			ticker.Stop()
			return nil
		case <-ticker.C:
			this.cb()
		}
	}
}

func (this *NetEvent) EventLoop() int {
	logrus.Info("Start Main EventLoop ")
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

func (this *NetEvent) ScheduleTimer(ms int64, cb func() error) int64 {
	timeOut := &TimeOut{
		msInterval: time.Duration(ms) * time.Millisecond,
		cb:         cb,
		quit:       make(chan bool),
		ne:         this,
	}
	id := atomic.AddInt64(&this.timeOutIDGen, 1)
	this.timeOutMap.Store(id, timeOut)
	go timeOut.Run()
	return id
}
func (this *NetEvent) CancelTimer(id int64) {
	to, ok := this.timeOutMap.Load(id)
	if !ok {
		return
	}
	to.(TimeOut).quit <- true
	this.timeOutMap.Delete(id)
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
