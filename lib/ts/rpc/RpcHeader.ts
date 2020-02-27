import  * as rpc from "../rpc/index"
import  {MsgProtocolType}  from "./MsgProtocolType"
export class RpcHeader
{ 
	public static readonly strFingerprint:string="2bcc93ddec2a7ac9beece683a78b5561"
	public version:number=0
	public msgType:MsgProtocolType=0
	public serviceName:string=""
	//serialize
	public serialize( __P__:rpc.Protocol):void 
	{ 
		__P__.writeInt8(this.version);

		__P__.writeInt16(this.msgType);

		__P__.writeString(this.serviceName);

	}// serialize
	//deSerialize
	public deSerialize( __P__:rpc.Protocol):void 
	{ 
		this.version=__P__.readInt8()

		this.msgType=__P__.readInt16()

		this.serviceName=__P__.readString()

	}// deSerialize
}//class
