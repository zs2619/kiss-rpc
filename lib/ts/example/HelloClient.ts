import * as rpc from "../rpc/index"
import {opServiceStub} from "../shuai/index"
let ch =new rpc.RpcChannel("ws://121.40.165.18:8800",rpc.WsTransport,rpc.BinaryProtocol)
let stub=ch.createStub(opServiceStub)
stub.login('shuai',(i8:number):number=>{
	return 0
}
)
console.log(ch)
console.log(stub)