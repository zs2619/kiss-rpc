//==============================================
/**
	@date:		2018:11:07 
	@file: 		RpcMessage.h
	@author		zhangshuai
*/
//==============================================

#ifndef	__RPC_RPCMSG_H__
#define	__RPC_RPCMSG_H__
#include "Common.h"
#include <chrono>
#include "rpc/Packet.h"
namespace rpc {
const uint16 MsgHeaderMaxSize=2;
class RpcMsg {
public:
	RpcMsg() {}
	~RpcMsg() {}
	std::string serviceName_;
	std::string functionName_;
	std::chrono::system_clock::time_point time_;
	RequestMsg 	 requestMsg_;
	ResponseMsg  responseMsg_;
};
}
#endif