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

	class RpcMsg {
	public:
		RpcMsg() {}
		~RpcMsg() {}
		std::chrono::system_clock::time_point time_;
		RequestMsg 	 requestMsg_;
		ResponseMsg  responseMsg_;
	};
}
#endif