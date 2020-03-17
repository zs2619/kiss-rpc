package kissnet

import (
	"bytes"
	"sync/atomic"

	"github.com/gorilla/websocket"
)

// websocket会话
type WsConnection struct {
	id       int64 //会话id
	conn     *websocket.Conn
	_isClose int32
	sendCh   chan *bytes.Buffer
	acceptor *WSAcceptor
}

func (this *WsConnection) getId() int64 {
	return this.id
}
func (this *WsConnection) setID(id int64) {
	this.id = id
}

func newWSConnection(conn *websocket.Conn, acceptor *WSAcceptor) IConnection {
	c := &WsConnection{
		conn:     conn,
		_isClose: 0,
		sendCh:   make(chan *bytes.Buffer, 1024),
		acceptor: acceptor,
	}
	return c
}

func (this *WsConnection) IsClose() bool { return atomic.LoadInt32(&this._isClose) > 0 }

func (this *WsConnection) Close() {
}

func (this *WsConnection) SendMsg(msg *bytes.Buffer) bool {
	if this.IsClose() {
		//关闭不能发送消息
		return false
	}
	//推入发送循环
	this.sendCh <- msg
	return true
}

func (this *WsConnection) GetId() int64 {
	return this.id
}

func (this *WsConnection) SetID(id int64) {
	this.id = id
}

func (this *WsConnection) start() {
	//开启读写 goroutine
	go this.recvMsgLoop()
	go this.sendMsgLoop()
}

func (this *WsConnection) sendMsgLoop() {

	for msg := range this.sendCh {
		if msg == nil || this.conn == nil {
			return
		}
		this.conn.WriteMessage(websocket.BinaryMessage, msg.Bytes())

	}
}

func (this *WsConnection) recvMsgLoop() {

	for {
		if this.conn == nil {
			break
		}
		mt, _, err := this.conn.ReadMessage()
		if err != nil {
			break
		}
		if mt == websocket.BinaryMessage {
		}
	}
	// 退出处理
	this.Close()
}
