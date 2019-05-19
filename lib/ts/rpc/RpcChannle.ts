import {ServiceStub} from "./ServiceStub"

interface dummy extends ServiceStub
{
	getObjName():string,
}

export class RpcChannel{
	private endPoint:string
	private ws:WebSocket
	private stubMap:Map<string,ServiceStub>=new Map<string,ServiceStub>()

	constructor(endPoint:string){
		this.endPoint=endPoint
		this.ws = new WebSocket(this.endPoint)
		let that = this;
		this.ws.onopen = function (ev: Event) { 
			if (that.onConnect) { that.onConnect.call(that,ev); }
            };
	}
	public  createStub<T extends dummy>(ctor: { new(c:RpcChannel): T }):T{
		let t= new ctor(this);
		this.stubMap.set(t.getObjName(),t)
		return t
	}

	private  onConnect(ev: Event){
		console.log("WebSocket is open now.")
		let that = this;
		that.send('shuai')
		that.ws.onmessage = function (ev: Event) { 
				if (that.onMessage) { that.onMessage.call(that,ev); }
            };
		that.ws.onclose = function (ev: Event) { 
				if (that.onClose) { that.onClose.call(that,ev); }
			};
		that.ws.onerror = function (ev: Event) { 
				if (that.onError) { that.onError.call(that,ev); }
            };
	}
	private onMessage(ev:Event){
		console.log("WebSocket message received:", ev);
	}
	private onClose(ev:Event){
		console.log("WebSocket close:", ev);
	}
	private onError(ev:Event){
		console.log("WebSocket error:", ev);
	}
	private send(str:string){
		this.ws.send(str)
	}
	private handleInput(){

	}

}