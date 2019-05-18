import * as rpc from "../rpc/index"
import {OpServiceStub} from "../shuai/index"
let ch =new rpc.RpcChannel("ws://127.0.0.1:2619")
ch.createStub(OpServiceStub)
console.log(ch)