//==============================================
/**
	@date:		2018:11:07 
	@file: 		RpcMessage.h
	@author		zhangshuai
*/
//==============================================

#include "Common.h"

class RequestMsg{
public:	
	int msgSeqId;
	std::vector<char> buf;
	int msgId;
};
class ReponesMsg{
public:	
	int msgSeqId;
	int msgId;
	std::vector<char> buf;
};

class RpcMsg {
public:
	RpcMsg()  {}

	RequestMsg 	sendMsg;
	ReponesMsg 	recvMsg;

	static std::map<int,RpcMsg*> RpcMsgMap;
};