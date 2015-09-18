package rpclib

import (
	"fmt"
	"net"
	"sync"
	"time"
)

type RpcServer struct {
	Address        string
	SessionFactory ISessionFactory
	Ch             chan int
	sync.Mutex
	SessionMap map[int64]*ISession
}

func (this *RpcServer) Serve() error {
	fmt.Println("rpc server listen")
	ln, err := net.Listen("tcp", this.Address)
	if err != nil {
		return err
	}
	fmt.Println("rpc server start")
	defer ln.Close()

	go this.Update()

	var i int64
	for {
		c, err := ln.Accept()
		if err != nil {
			return err
		}
		i++
		ch := make(chan int64)
		s := this.SessionFactory.NewSession(i, &c, &ch)
		this.AddSession(s)
		go s.HandleSession()
	}
	fmt.Println("rpc server end")
	return nil
}

func (this *RpcServer) AddSession(s ISession) bool {
	this.Lock()
	defer this.Unlock()

	if _, exist := this.SessionMap[s.GetId()]; exist {
		return false
	}
	this.SessionMap[s.GetId()] = &s
	return true
}

func (this *RpcServer) DelSession(id int64) {
	this.Lock()
	defer this.Unlock()
	delete(this.SessionMap, id)
}

func (this *RpcServer) Close() {
	this.Lock()
	defer this.Unlock()
	for i, v := range this.SessionMap {
		(*v).Close()
		delete(this.SessionMap, i)
	}

}

func (this *RpcServer) Update() {

	for {
		time.Sleep(1)
		this.Lock()
		for i, v := range this.SessionMap {
			select {
			case <-(*(*v).GetServerCh()):
				delete(this.SessionMap, i)
			default:
			}
		}
		this.Unlock()
	}
}

func NewRpcServer(addr string, SessionFactory ISessionFactory) *RpcServer {
	server := &RpcServer{Address: addr, SessionMap: make(map[int64]*ISession), SessionFactory: SessionFactory, Ch: make(chan int)}
	return server
}
