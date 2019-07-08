import {RequestMsg} from "./RequestMsg"
import {ResponseMsg} from "./ResponseMsg"

export abstract  class Transport {
	constructor(){
	}

	public abstract connect(endPoint:string):boolean
	public abstract sendRequestMsg(reqMsg:RequestMsg):boolean
	public abstract recvResponseMsg(buff:Uint8Array,  msgVec:ResponseMsg[]):boolean

	public abstract sendResponseMsg(respMsg:ResponseMsg) :boolean
	public abstract recvRequestMsg( buff:Uint8Array, msgVec:RequestMsg[]):boolean

	public handleConnect: ((ev: Event) => boolean) | null=null;
    public onMessage: (( buff:Uint8Array) => boolean) | null=null;
	public onClose: ((ev: Event) => boolean) | null=null;

	public setCallBack(connCB:(ev: Event) => boolean,inputCB:( buff:Uint8Array) => boolean,closeCB:(ev: Event) => boolean){
		this.handleConnect=connCB
		this.onMessage=inputCB
		this.onClose=closeCB
	}
}

export class WsTransport extends Transport {
	private ws!:WebSocket
	constructor(){
		super()
	}

	public  connect(endPoint:string):boolean{
		this.ws = new WebSocket(endPoint)
		let that = this;
		this.ws.onopen = function (ev: Event) { 
			if (that.onOpen) { that.onOpen.call(that,ev) }
			}

		return true
	}

	private  onOpen(ev: Event){
		console.log("WebSocket is open now.")
		if (this.handleConnect===null){
			console.log("WebSocket is onConnect null.")
			return 
		}
		this.handleConnect(ev)
		let that = this;
		that.ws.onmessage = function (ev: MessageEvent) { 
				if (that.onMessage) { that.onMessage.call(that,ev.data); }
            };
		that.ws.onclose = function (ev: Event) { 
				if (that.onClose) { that.onClose.call(that,ev); }
			};
		that.ws.onerror = function (ev: Event) { 
				if (that.onClose) { that.onClose.call(that,ev); }
            };
	}

	private send(buff:Uint8Array){
		this.ws.send(buff)
	}

	public sendRequestMsg(reqMsg:RequestMsg):boolean
	{
		return true
	}
	public recvResponseMsg(buff:Uint8Array,  msgVec:ResponseMsg[]):boolean
	{
		return true
	}
	public sendResponseMsg(respMsg:ResponseMsg) :boolean
	{
		return true
	}
	public recvRequestMsg( buff:Uint8Array, msgVec:RequestMsg[]):boolean
	{
		return true
	}
}
export class HttpTransport extends Transport {

	public  connect(endPoint:string):boolean{
		return true
	}
	public sendRequestMsg(reqMsg:RequestMsg):boolean
	{
		return true
	}
	public recvResponseMsg(buff:Uint8Array,  msgVec:ResponseMsg[]):boolean
	{
		return true
	}
	public sendResponseMsg(respMsg:ResponseMsg) :boolean
	{
		return true
	}
	public recvRequestMsg( buff:Uint8Array, msgVec:RequestMsg[]):boolean
	{
		return true
	}
}