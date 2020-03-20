package kissnet

import (
	"bytes"
	"encoding/binary"
	"io"
	"net"
	"sync"
)

type IConnection interface {
	start()
	getId() int64
	setID(id int64)
}

const MsgHeaderMaxSize = 2

type Connection struct {
	id       int64
	conn     *net.TCPConn
	exitSync sync.WaitGroup
	sendCh   chan *bytes.Buffer
}

func newConnection(conn *net.TCPConn) *Connection {
	c := &Connection{
		conn:   conn,
		sendCh: make(chan *bytes.Buffer, 1024),
	}
	return c
}

func (this *Connection) getId() int64 {
	return this.id
}
func (this *Connection) setID(id int64) {
	this.id = id
}

func (this *Connection) start() {
	this.conn.SetNoDelay(true)
	this.conn.SetKeepAlive(true)

	//同步退出 goroutine
	this.exitSync.Add(2)

	//开启读写 goroutine
	go this.recvMsgLoop()
	go this.sendMsgLoop()
}

func (this *Connection) SendMsg(msg *bytes.Buffer) bool {
	this.sendCh <- msg
	return true
}

func (this *Connection) sendMsgLoop() {
	for msg := range this.sendCh {
		if msg == nil || this.conn == nil {
			break
		}

		msgLen := uint16(msg.Len())
		buf := make([]byte, MsgHeaderMaxSize)
		binary.LittleEndian.PutUint16(buf, msgLen)
		_, err := this.conn.Write(buf)
		if err != nil {
			break
		}
		_, err = this.conn.Write(msg.Bytes())
		if err != nil {
			break
		}
	}
	//关闭socket 从读操作退出
	this.shutDown()
	this.exitSync.Done()
}

func (this *Connection) recvMsgLoop() {
	var err error
	var msgLen int
	msgHeader := make([]byte, MsgHeaderMaxSize)
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
	}
	this.exitSync.Done()
}

func (this *Connection) shutDown() {
	// 关闭连接
	if this.conn != nil {
		this.conn.Close()
		this.conn = nil
	}
}
