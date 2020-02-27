import {RpcChannel} from  "./RpcChannle"
import {ResponseMsg} from  "./ResponseMsg"
import {RpcMsg} from  "./RpcMessage"
import {MsgProtocolType} from  "./MsgProtocolType"
import {Int64} from  "./BigInt"
import { Connection } from "./Connection";

export abstract class ServiceStub{
	public msgQueue:Map<Int64,RpcMsg>=new Map<Int64,RpcMsg>()
	maxMsgSeqId=0;
	protected chan:Connection
	constructor(chan:Connection){
		this.chan=chan
	}

	public stubMsgCallBack(respMsg:ResponseMsg):boolean{
		let rpcMsg=this.msgQueue.get(respMsg.msgSeqId)
		if (rpcMsg==undefined){
			return false
		}
		rpcMsg.responseMsg=respMsg
		this.dispatch(rpcMsg)
		this.msgQueue.delete(respMsg.msgSeqId)
		return true
	}

	protected abstract dispatch( msg:RpcMsg):boolean

	protected invoke(msg:RpcMsg):boolean{
		msg.time= new Date().getTime()
		msg.requestMsg.header.version=0;
		msg.requestMsg.header.msgType=MsgProtocolType.MPT_Bin;
		msg.requestMsg.header.serviceName=msg.serviceName;
		let ret= this.chan.getTransport().sendRequestMsg(msg.requestMsg)
		if (!ret)
		{
			console.log('ServiceStub invoke sendRequestMsg error')
		}

		this.msgQueue.set(msg.requestMsg.msgSeqId,msg)
		return true
	};
}