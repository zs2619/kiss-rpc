package rpclib

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"net"
)

type ISessionFactory interface {
	NewSession(id int64, conn *net.Conn, serverCh *chan int64) ISession
}
type ISession interface {
	ProcessMsg(b *bytes.Buffer)
	HandleSession()
	GetId() int64
	GetServerCh() *chan int64
	Close()
}

type hanldeMsg func(b *bytes.Buffer) bool
type RpcSession struct {
	Id       int64
	Conn     *net.Conn
	ServerCh *chan int64
	SendCh   chan *bytes.Buffer
	RecvCh   chan *bytes.Buffer
}

func (this *RpcSession) Init() {
	//
	go this.sendLoop()
}

func (this *RpcSession) Close() {
	(*this.ServerCh) <- this.Id
	(*this.Conn).Close()
	close(this.SendCh)
	close(this.RecvCh)
	close(*this.ServerCh)

}

func (this *RpcSession) GetId() int64 {
	return this.Id
}
func (this *RpcSession) GetServerCh() *chan int64 {
	return this.ServerCh
}

func (this *RpcSession) Send(b *bytes.Buffer) error {
	this.SendCh <- b
	return nil
}
func (this *RpcSession) sendLoop() error {
	for buf := range this.SendCh {
		(*(this.Conn)).Write(buf.Bytes())
	}
	return nil
}

func (this *RpcSession) ProcessMsg(b *bytes.Buffer) {
	this.RecvCh <- b
}

func (this *RpcSession) HandleSession() {
	fmt.Println("server handleConnection start", this.Id)

	defer this.Close()

	///消息处理
	buf := new(bytes.Buffer)
	rb := make([]byte, 1024*1024)
	for {
		n, err := (*this.Conn).Read(rb)
		if err != nil {
			fmt.Println(err.Error())
			break
		}

		buf.Write(rb[0:n])
		for {
			if buf.Len() > 2 {
				var msgLen uint16
				binary.Read(buf, binary.LittleEndian, &msgLen)
				if int(msgLen) <= buf.Len() {
					///大于一条完整消息
					msg := buf.Next(int(msgLen))
					this.ProcessMsg(bytes.NewBuffer(msg))
				} else {
					break
				}

			} else {
				break
			}

			buf = bytes.NewBuffer(buf.Bytes())
		}
	}
	fmt.Println("server handleConnection end ", this.Id)
}
