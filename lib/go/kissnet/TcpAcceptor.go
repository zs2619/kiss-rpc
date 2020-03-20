package kissnet

import (
	"fmt"
	"kiss/gateway/config"
	"net"

	"github.com/sirupsen/logrus"
)

type IAcceptor interface {
	Run() error
	Close() error
}

func AcceptorFactory(cfg *config.GWConfig) (IAcceptor, error) {
	if cfg.AcceptorType == "ws" {
		return NewWSAcceptor(cfg)
	} else if cfg.AcceptorType == "tcp" {
		return NewTcpAcceptor(cfg)
	} else {
		return nil, fmt.Errorf("config AcceptorType error")
	}
}

type Acceptor struct {
	cMgr ConnectionMgr
}
type TcpAcceptor struct {
	Acceptor
	listener *net.TCPListener
	Addr     *net.TCPAddr
}

func NewTcpAcceptor(cfg *config.GWConfig) (*TcpAcceptor, error) {
	ep := fmt.Sprintf("0.0.0.0:%d", cfg.Port)
	tcpAddr, err := net.ResolveTCPAddr("tcp", ep)
	if err != nil {
		return nil, err
	}
	ln, err := net.ListenTCP("tcp", tcpAddr)
	if err != nil {
		return nil, err
	}
	return &TcpAcceptor{Addr: tcpAddr, listener: ln}, nil

}
func (this *TcpAcceptor) Run() error {
	go this.accept()
	return nil
}

func (this *TcpAcceptor) accept() {
	for {
		tcpConn, err := this.listener.AcceptTCP()
		if err != nil {
			logrus.WithFields(logrus.Fields{
				"error": err,
			}).Warn("TcpAcceptor::accept error")
			return
		}
		conn := newConnection(tcpConn)
		go conn.start()
	}
}

func (this *TcpAcceptor) Close() error {
	this.listener.Close()
	return nil
}
