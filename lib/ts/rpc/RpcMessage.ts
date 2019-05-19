import { RequestMsg} from "./RequestMsg"
import { ResponseMsg} from "./ResponseMsg"

export class RpcMsg {
	public serviceName:string=""
	public functionName:string=""
    public time:number=0
    public requestMsg:RequestMsg=new RequestMsg
	public responseMsg:ResponseMsg=new ResponseMsg
};