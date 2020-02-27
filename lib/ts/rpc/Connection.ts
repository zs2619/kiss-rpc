import {Protocol} from "./Protocol"
import {Transport} from "./Transport"
import {ResponseMsg} from "./ResponseMsg"


export abstract class Connection {
	private trans!:Transport;
	private proto!:Protocol;

	protected endPoint:string
	public getProtocol():Protocol    {return this.proto;}
	public getTransport():Transport  {return this.trans;}

	constructor(endPoint:string,trans:Transport,proto:Protocol){
		this.endPoint=endPoint
		this.trans=trans
		this.proto=proto
	}

	public abstract handleInput(buff:Uint8Array):boolean
	public abstract handleClose(ev: Event):boolean
}