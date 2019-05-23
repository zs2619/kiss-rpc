import  * as rpc from "../rpc/index"
import  {RpcHeader}  from "./RpcHeader"
export class RequestMsg
{ 
	public static readonly strFingerprint:string="e61c37380db853addbaafae6c4850b2e"
	public header:RpcHeader=new RpcHeader()
	public msgSeqId:rpc.Int64=new rpc.Int64
	public msgId:number=0
	public buff!:Uint8Array
	//serialize
	public serialize( __P__:rpc.Protocol):void 
	{ 
		this.header.serialize(__P__);

		__P__.writeInt64(this.msgSeqId);

		__P__.writeInt32(this.msgId);

		__P__.writeBinary(this.buff);

	}// serialize
	//deSerialize
	public deSerialize( __P__:rpc.Protocol):void 
	{ 
		this.header.deSerialize(__P__)

		this.msgSeqId=__P__.readInt64()

		this.msgId=__P__.readInt32()

		this.buff=__P__.readBinary()

	}// deSerialize
}//class
