import * as rpc from "../rpc/index"
import {OpServiceStub} from "../shuai/index"
let ch =new rpc.RpcChannel("ws://121.40.165.18:8800")
let stub=ch.createStub(OpServiceStub)
console.log(ch)
console.log(stub)