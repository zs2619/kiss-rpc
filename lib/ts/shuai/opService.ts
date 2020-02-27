import  * as rpc from "../rpc/index"
import  {user}  from "./user"
import  {role}  from "./role"
export class opServiceStub extends rpc.ServiceStub
{ 
	static readonly strFingerprint="878e3bfc527601565994f11bb9d347d0"
	static readonly getObjName="shuai"
	public getObjName():string{ return opServiceStub.getObjName }
	private loginCallBack!:(ret:number)=>number
	private xixiCallBack!:(ret:user)=>number
	private lalaCallBack!:(ret:role[])=>number
	private pingCallBack!:(ret:number)=>number
	constructor(chan:rpc.Connection)
	{ 
		super(chan)
	} 
	protected invokeAsync(msgId:number,p:rpc.Protocol, functionName:string) {
		let msg =new rpc.RpcMsg();
		msg.serviceName= this.getObjName();
		msg.functionName= functionName ;
		msg.requestMsg.msgId = msgId;
		msg.requestMsg.buff = p.getBuffer();
		super.invoke(msg);
	}
	public login(openid:string,cb:(ret:number)=>number)
	{
		let __P__= this.chan.getProtocol().createProtoBuffer();
		__P__.writeString(openid);

		this.invokeAsync(0,__P__,"login");
		this.loginCallBack=cb
	}
	public xixi(u:user,cb:(ret:user)=>number)
	{
		let __P__= this.chan.getProtocol().createProtoBuffer();
		u.serialize(__P__);

		this.invokeAsync(1,__P__,"xixi");
		this.xixiCallBack=cb
	}
	public lala(m:Map<number,role>,ai:number[],ar:role[],cb:(ret:role[])=>number)
	{
		let __P__= this.chan.getProtocol().createProtoBuffer();
		__P__.writeInt32(m.size);
		m.forEach((value , key) =>{ 
			__P__.writeInt32(key);
			value.serialize(__P__);
		})

		__P__.writeInt32(ai.length);
		for (let  temp of ai)
		{
			__P__.writeInt32(temp);
		}

		__P__.writeInt32(ar.length);
		for (let  temp of ar)
		{
			temp.serialize(__P__);
		}

		this.invokeAsync(2,__P__,"lala");
		this.lalaCallBack=cb
	}
	public ping(i8:number,cb:(ret:number)=>number)
	{
		let __P__= this.chan.getProtocol().createProtoBuffer();
		__P__.writeInt8(i8);

		this.invokeAsync(3,__P__,"ping");
		this.pingCallBack=cb
	}
	protected dispatch(msg:rpc.RpcMsg):boolean
	{
		let id=msg.responseMsg.msgId;
		switch (id)
		{
				case 0:
				{
					let __P__= this.chan.getProtocol().createProtoBuffer()
					__P__.setBuffer(msg.responseMsg.buff);
					let ret:number=0
					ret=__P__.readInt8()
					let cbRet = this.loginCallBack(ret);
					return true;
				}
				case 1:
				{
					let __P__= this.chan.getProtocol().createProtoBuffer()
					__P__.setBuffer(msg.responseMsg.buff);
					let ret:user=new user()
					ret.deSerialize(__P__)
					let cbRet = this.xixiCallBack(ret);
					return true;
				}
				case 2:
				{
					let __P__= this.chan.getProtocol().createProtoBuffer()
					__P__.setBuffer(msg.responseMsg.buff);
					let ret:role[]=[]
					let _n_2_array:number=__P__.readInt32()
					for( let _i_2_:number=0; _i_2_<_n_2_array; _i_2_++) {
						let tmp:role=new role()
						tmp.deSerialize(__P__)
						ret.push(tmp)
					}
					let cbRet = this.lalaCallBack(ret);
					return true;
				}
				case 3:
				{
					let __P__= this.chan.getProtocol().createProtoBuffer()
					__P__.setBuffer(msg.responseMsg.buff);
					let ret:number=0
					ret=__P__.readInt8()
					let cbRet = this.pingCallBack(ret);
					return true;
				}
				default:
				{
					return false;
				}
			}
		}
	};//class
