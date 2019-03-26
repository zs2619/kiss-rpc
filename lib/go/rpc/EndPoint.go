package rpc

type endPoint struct {
	hostName string
}

func (this *endPoint) GetPort() (string, error) {
	return "", nil
}
func (this *endPoint) GetHostName() string {
	return ""
}

func NewEndPoint(addr string) *endPoint {
	return &endPoint{hostName: addr}
}
