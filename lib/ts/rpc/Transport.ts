import {RequestMsg} from "./RequestMsg"
import {ResponseMsg} from "./ResponseMsg"
import {Connection} from "./Connection"

export abstract  class Transport {
	protected conn:Connection
	constructor(conn:Connection){
		this.conn=conn
	}

	public abstract connect(endPoint:string):boolean
	public abstract sendRequestMsg(reqMsg:RequestMsg):boolean
	public abstract recvResponseMsg(buff:Uint8Array,  msgVec:ResponseMsg[]):boolean
	public abstract sendResponseMsg(respMsg:ResponseMsg) :boolean
	public abstract recvRequestMsg( buff:Uint8Array, msgVec:RequestMsg[]):boolean

    public onResponseMsg: (( msgVec:ResponseMsg[]) => boolean) | null=null;
    public onRequestMsg: (( msgVec:RequestMsg[]) => boolean) | null=null;
}

export class WsTransport extends Transport {
	private ws!:WebSocket
	constructor(conn:Connection){
		super(conn)
	}

	public  connect(endPoint:string):boolean{
		this.ws = new WebSocket(endPoint)

		let that = this;
		this.ws.onopen = function (ev: Event) { 
			if (that.onConnect) { that.onConnect.call(that,ev); }
			};

		return true
	}

	private  onConnect(ev: Event){
		console.log("WebSocket is open now.")
		let that = this;
		that.send('shuai')
		that.ws.onmessage = function (ev: MessageEvent) { 
				if (that.onMessage) { that.onMessage.call(that,ev); }
            };
		that.ws.onclose = function (ev: Event) { 
				if (that.onClose) { that.onClose.call(that,ev); }
			};
		that.ws.onerror = function (ev: Event) { 
				if (that.onError) { that.onError.call(that,ev); }
            };
	}

	private onMessage(ev:MessageEvent){
		let buff= new Uint8Array(ev.data)
		let  respMsgList:ResponseMsg[]=[];
		let ret=this.recvResponseMsg(buff,respMsgList)
		if (ret)
			return
		
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