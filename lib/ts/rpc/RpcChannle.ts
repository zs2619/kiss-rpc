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
		super(endPoint,new tctor(),new pctor())

		let that=this
		this.getTransport().setCallBack(
			function (ev: Event):boolean{
				return that.handleConnect.call(that,ev)
			},

			function (buff:Uint8Array):boolean{
				return that.handleInput.call(that,buff)
			},

			function (ev: Event):boolean{
				return that.handleClose.call(that,ev)
			}
		)
		this.getTransport().connect(endPoint)
	}

	public  createStub<S extends dummy>(ctor: { new(c:RpcChannel<T,P>): S }):S{
		let t= new ctor(this);
		this.stubMap.set(t.getObjName(),t)
		return t
	}

	public handleInput(buff:Uint8Array):boolean{
		let  respMsgList:ResponseMsg[]=[];
		let ret=this.getTransport().recvResponseMsg(buff,respMsgList)
		if (ret)
			return false

		for(let msg of respMsgList)
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
	public handleClose(ev: Event):boolean{
		return true
	}
	public handleConnect(ev: Event):boolean{
		return true
	}
}