import {RpcChannel} from  "./RpcChannle"
import {ResponseMsg} from  "./ResponseMsg"
import {RpcMsg} from  "./RpcMessage"
import {MsgProtocolType} from  "./MsgProtocolType"
import {Int64} from  "./BigInt"



export  interface IServiceStub{
	dispatch( msg:RpcMsg):boolean
}
export class ServiceStub{
	public msgQueue:Map<Int64,RpcMsg>=new Map<Int64,RpcMsg>()
	maxMsgSeqId=0;
	protected chan:RpcChannel
	constructor(chan:RpcChannel){
		this.chan=chan
	}
	protected stubMsgCallBack(respMsg:ResponseMsg){
	}

	protected invoke(msg:RpcMsg):boolean{
		msg.time= new Date().getTime()
		msg.requestMsg.header.version=0;
		msg.requestMsg.header.msgType=MsgProtocolType.MPT_Bin;
		msg.requestMsg.header.serviceName=msg.serviceName;
		this.msgQueue.set(msg.requestMsg.msgSeqId,msg)
		return true
	};
}