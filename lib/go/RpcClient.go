package rpclib

import (
	// "encoding/binary"
	// "fmt"
	"net"
)

type RpcClient struct {
	Address        string
	SessionFactory ISessionFactory
	Id             int64
}

func (this *RpcClient) Connect() (ISession, error) {
	conn, err := net.Dial("tcp", this.Address)
	if err != nil {
		return nil, err
	}
	ch := make(chan int64)
	this.Id++
	s := this.SessionFactory.NewSession(this.Id, &conn, &ch)
	go s.HandleSession()
	return s, nil
}

func (this *RpcClient) Close() {
}

func NewRpcClient(addr string, SessionFactory ISessionFactory) *RpcClient {
	client := &RpcClient{Address: addr, SessionFactory: SessionFactory}
	return client
}
