import {ServiceStub} from "./ServiceStub"

interface dummy extends ServiceStub
{
	getObjName:string,
}

export class RpcChannel{
	private endPoint:string
	private ws:any
	private stubMap:Map<string,ServiceStub>=new Map<string,ServiceStub>()

	constructor(endPoint:string){
		this.endPoint=endPoint
		this.ws = new WebSocket(this.endPoint)
		this.ws.onopen =this.onConnect
	}
	public  createStub<T extends dummy>(ctor: { new(c:RpcChannel): T }):T{
		let t= new ctor(this);
		this.stubMap.set(t.getObjName,t)
		return t
	}

	private  onConnect(ev: Event){
		console.log("WebSocket is open now.")
		this.ws.onmessage =this.onMessage
		this.ws.onclose=this.onclose
		this.ws.onerror=this.onError
	}
	private onMessage(ev:Event){
		console.debug("WebSocket message received:", ev);
	}
	private onclose(ev:Event){

	}
	private onError(ev:Event){

	}
	private send(){

	}
	private handleInput(){

	}

}