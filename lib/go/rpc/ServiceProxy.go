package rpc


type ServiceProxy struct {
	*RpcService
}

func (this *ServiceProxy) Invoke(msg *RpcMsg) int {
	msg.ResponseMsg.MsgId=msg.RequestMsg.MsgId;
	msg.ResponseMsg.MsgSeqId=msg.RequestMsg.MsgSeqId;
	msg.ResponseMsg.Header.Version=0;
	msg.ResponseMsg.Header.MsgType=MPT_Bin;
	msg.ResponseMsg.Header.ServiceName=msg.RequestMsg.Header.ServiceName;

	ret:=this.GetTransport().sendResponseMsg(&msg.ResponseMsg)
	if ret!=nil{
		return -1;
	}
	return 0
}

func NewServiceProxy(rpcService *RpcService) *ServiceProxy {
	proxy := &ServiceProxy{RpcService: rpcService}
	return proxy
}
