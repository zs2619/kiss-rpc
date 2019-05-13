import  * as rpc from "../rpc/index"
import  {SexType}  from "./SexType"
export class role
{ 
	public static readonly strFingerprint:string="5ad2cb5c4c22f48680d17d2a084021b2"
	public sexType:SexType=0
	public b:boolean=false
	public by:number=0
	public i8:number=0
	public i16:number=0
	public i32:number=0
	public i64:rpc.Int64=new rpc.Int64
	public f:number=0.0
	public str:string=""
	public bin:Uint8Array|null=null
	//serialize
	public serialize( __P__:rpc.IProtocol):void 
	{ 
		__P__.writeInt16(this.sexType);

		__P__.writeBool(this.b);

		__P__.writeByte(this.by);

		__P__.writeInt8(this.i8);

		__P__.writeInt16(this.i16);

		__P__.writeInt32(this.i32);

		__P__.writeInt64(this.i64);

		__P__.writeFloat(this.f);

		__P__.writeString(this.str);

		__P__.writeBinary(this.bin);

	}// serialize
	//deSerialize
	public deSerialize( __P__:rpc.IProtocol):void 
	{ 
		this.sexType=__P__.readInt16()

		this.b=__P__.readBool()


		this.i8=__P__.readInt8()

		this.i16=__P__.readInt16()

		this.i32=__P__.readInt32()

		this.i64=__P__.readInt64()

		this.f=__P__.readFloat()

		this.str=__P__.readString()

		this.bin=__P__.readBinary()

	}// deSerialize
}//class
