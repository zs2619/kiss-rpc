#ifndef	__PACKET_H__
#define	__PACKET_H__

#include <functional>
#include "rpc/Common.h"
#include "rpc/Protocol.h"
namespace rpc{

class RequestMsg
{ 
public: 
	RequestMsg();
	virtual ~RequestMsg();
	static const char* strFingerprint;
	rpc::int64	msgSeqId;
	rpc::uint16	msgId;
	std::vector<rpc::int8> 	buff;

	//serialize
	void serialize(rpc::IProtocol* __P__) const ;

	//deSerialize
	bool deSerialize(rpc::IProtocol* __P__);
} ;//struct

class ResponseMsg
{ 
public: 
	ResponseMsg();
	virtual ~ResponseMsg();
	static const char* strFingerprint;
	rpc::int64	msgSeqId;
	rpc::uint16	msgId;
	std::vector<rpc::int8> 	buff;

	//serialize
	void serialize(rpc::IProtocol* __P__) const ;

	//deSerialize
	bool deSerialize(rpc::IProtocol* __P__);
} ;//struct

}///namespace
#endif
