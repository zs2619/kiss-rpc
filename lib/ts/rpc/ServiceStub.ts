import {RpcChannel} from  "./RpcChannle"
export class ServiceStub{
	protected chan:RpcChannel
	constructor(chan:RpcChannel){
		this.chan=chan
	}
}