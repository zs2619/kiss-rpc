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

class RequestMsg{
public:	
	int msgSeqId;
	std::vector<int8> buf;
	int msgId;
};
class ResponseMsg{
public:	
	int msgSeqId;
	int msgId;
	std::vector<int8> buf;
};

class RpcMsg {
public:
	RpcMsg()  {}
	RequestMsg 	 sendMsg_;
	ResponseMsg  recvMsg_;

	static std::map<int,RpcMsg*> RpcMsgMap;
};
#endif