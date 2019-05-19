import {RpcChannel} from  "./RpcChannle"
export class ServiceStub{
	protected chan:RpcChannel
	constructor(chan:RpcChannel){
		this.chan=chan
	}

	protected stubMsgCallBack( respMsg:ResponseMsg){
	}
	public bool dispatch(std::shared_ptr<rpc::RpcMsg> msg);
}