package rpc

import (
	"bytes"
	"encoding/binary"
	"io"
)

type ITransport interface {
	sendRequestMsg(reqMsg *RequestMsg) error
	recvResponseMsg(buff *bytes.Buffer) (*ResponseMsg, error)
	sendResponseMsg(respMsg *ResponseMsg) error
	recvRequestMsg(buff *bytes.Buffer) (*RequestMsg, error)
	start(conn *connection) error
}
type ITransportFactory interface {
	NewTransport() ITransport
}

type TcpTransportFactory struct {
}

func (this *TcpTransportFactory) NewTransport() ITransport {
	return &TcpTransport{}
}

type TcpTransport struct {
	*connection
}

func (this *TcpTransport) sendRequestMsg(reqMsg *RequestMsg) error {
	var proto BinaryProtocol
	reqMsg.serialize(&proto)

	this.SendBuffer(proto.GetBuffer())
	return nil
}
func (this *TcpTransport) recvResponseMsg(buff *bytes.Buffer) (*ResponseMsg, error) {

	var proto BinaryProtocol
	proto.SetBuffer(buff)
	var msg ResponseMsg
	msg.deSerialize(&proto)
	return &msg, nil

}

func (this *TcpTransport) sendResponseMsg(respMsg *ResponseMsg) error {

	var proto BinaryProtocol
	respMsg.serialize(&proto)

	this.SendBuffer(proto.GetBuffer())

	return nil
}
func (this *TcpTransport) recvRequestMsg(buff *bytes.Buffer) (*RequestMsg, error) {

	var proto BinaryProtocol
	proto.SetBuffer(buff)
	var msg RequestMsg
	msg.deSerialize(&proto)
	return &msg, nil
}
func (this *TcpTransport) start(conn *connection) error {
	this.connection = conn

	this.conn.SetNoDelay(true)
	this.conn.SetKeepAlive(true)

	//同步退出 goroutine
	this.exitSync.Add(2)

	//开启读写 goroutine
	go this.recvMsgLoop()
	go this.sendMsgLoop()

	return nil
}

func (this *TcpTransport) SendBuffer(buff *bytes.Buffer) bool {

	//推入发送循环
	this.sendCh <- buff
	return true
}

func (this *TcpTransport) sendMsgLoop() {

	for msg := range this.sendCh {
		if msg == nil || this.conn == nil {
			break
		}

		buf := make([]byte, 2)
		_, err := this.conn.Write(buf)
		if err != nil {
			//关闭socket 从读操作退出
			this.shutDown()
			break
		}
	}
	this.exitSync.Done()
}

func (this *TcpTransport) recvMsgLoop() {
	var err error
	var msgLen int
	msgHeader := make([]byte, 2) //前2字节存msgLen
	for {
		_, err = io.ReadFull(this.conn, msgHeader)
		if err != nil {
			break
		}
		msgLen = int(binary.LittleEndian.Uint16(msgHeader))
		if msgLen <= 0 || msgLen > 65535 {
			break
		}

		msgBody := make([]byte, msgLen)
		_, err = io.ReadFull(this.conn, msgBody)
		if err != nil {
			break
		}
		cMsg := NewCommonMsg(this.cb, 1, bytes.NewBuffer(msgBody))
		this.event.queue <- cMsg
	}
	this.exitSync.Done()
}

func (this *TcpTransport) shutDown() {
	// 关闭连接
	if this.conn != nil {
		this.conn.Close()
		this.conn = nil
	}
}
