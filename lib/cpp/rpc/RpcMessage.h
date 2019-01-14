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

class RequestMsg{
public:	
	int64 msgSeqId;
	std::vector<int8> buf;
	int msgId;
};
class ResponseMsg{
public:	
	int64 msgSeqId;
	int msgId;
	std::vector<int8> buf;
};

class RpcMsg {
public:
	RpcMsg()  {}
	~RpcMsg()  {}
 	std::chrono::system_clock::time_point time_;
	RequestMsg 	 sendMsg_;
	ResponseMsg  recvMsg_;
};
#endif