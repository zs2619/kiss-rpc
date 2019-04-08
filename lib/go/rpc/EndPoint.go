package rpc

import "net"

type endPoint struct {
	hostName string
	tcpAddr  *net.TCPAddr
}

func (this *endPoint) GetPort() (string, error) {
	return "", nil
}
func (this *endPoint) GetHostName() string {
	return ""
}
func (this *endPoint) GetTcpAddr() *net.TCPAddr {
	return this.tcpAddr
}

func NewEndPoint(addr string) *endPoint {
	tcpAddr, err := net.ResolveTCPAddr("tcp", addr)
	if err != nil {
		return nil
	}
	return &endPoint{hostName: addr, tcpAddr: tcpAddr}
}
