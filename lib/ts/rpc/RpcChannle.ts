import {ServiceStub} from "./ServiceStub"
import {Connection} from "./Connection"
import {ResponseMsg} from "./ResponseMsg"
import {Protocol} from "./Protocol"
import {Transport} from "./Transport"


interface dummy extends ServiceStub
{
	getObjName():string,
}

export class RpcChannel<T extends Transport,P extends Protocol> extends Connection{
	private stubMap:Map<string,ServiceStub>=new Map<string,ServiceStub>()

	constructor(endPoint:string,tctor:{new(conn?:Connection):T},pctor:{new():P} ){
		super(endPoint,new tctor(this),new pctor())
		this.getTransport().connect(endPoint)
	}
	public  createStub<S extends dummy>(ctor: { new(c:RpcChannel<T,P>): S }):S{
		let t= new ctor(this);
		this.stubMap.set(t.getObjName(),t)
		return t
	}
	public handleInput(respMsgVec:ResponseMsg[]):boolean{
		for(let msg of respMsgVec)
		{
			let stub = this.stubMap.get(msg.header.serviceName);
			if (stub==undefined) {
				console.log("message stubMap get error");
				continue
			}
			let ret=stub.stubMsgCallBack(msg);
			if (ret)
			{
				console.log("message stubMsgCallBack error");
				return false 
			}
		}
		return true
	}
	public handleClose():boolean{
		return true
	}
}