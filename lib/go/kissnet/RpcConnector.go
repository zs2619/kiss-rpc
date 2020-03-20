package kissnet

import (
	"bytes"
	"kiss/gateway/config"
	"net"
)

type RpcConnectorMgr struct {
	RpcConnMap map[string]*RpcConnector
}

func (this *RpcConnectorMgr) Init(cfg *config.GWConfig) error {
	rc := &RpcConnector{}
	rc.start()
	this.addRpcConnector(rc)
	return nil
}

func (this *RpcConnectorMgr) addRpcConnector(c *RpcConnector) error {
	this.RpcConnMap[c.serviceName] = c
	return nil
}

type RpcConnector struct {
	serviceName string
	conn        *Connection
	addr        string
}

func (this *RpcConnector) start() error {
	this.connect()
	return nil
}

func (this *RpcConnector) connect() error {
	tcpAddr, err := net.ResolveTCPAddr("tcp", this.addr)
	if err != nil {
		return err
	}

	c, err := net.DialTCP("tcp", nil, tcpAddr)
	if err != nil {
		return err
	}
	this.conn = newConnection(c)
	this.conn.start()
	return nil
}
func (this *RpcConnector) sendMsg(buf *bytes.Buffer) error {
	return nil
}

func (this *RpcConnector) Close() error {
	return nil
}
