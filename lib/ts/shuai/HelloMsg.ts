import { ServiceStub,IProtocol,RpcMsg,IServiceStub} from "../rpc";

export class OpServiceStub extends ServiceStub implements IServiceStub{
	static readonly  strFingerprint:string="shuai"
	static readonly  getObjName:string="OpService"
	public getObjName():string{
		return OpServiceStub.getObjName
	}

	public invokeAsync( msgId:number, p:IProtocol,  functionName:string){

	}
	public dispatch( msg:RpcMsg):boolean{
		return true
	}
}