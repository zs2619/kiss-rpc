#ifndef	__PACKET_H__
#define	__PACKET_H__

#include <functional>
#include "rpc/Common.h"
#include "rpc/Protocol.h"
namespace rpc{

enum MsgProtocolType
{ 
	MPT_Bin,
	MPT_Json,
}; 
extern rpc::EnumMap RpcEnum(MsgProtocolType);

class RpcHeader
{ 
public: 
	RpcHeader();
	virtual ~RpcHeader();
	static const char* strFingerprint;
	rpc::uint8	version;
	MsgProtocolType	msgType;
	std::string	serviceName;

	//serialize
	void serialize(rpc::IProtocol* __P__) const ;

	//deSerialize
	bool deSerialize(rpc::IProtocol* __P__);
} ;//struct

class RequestMsg
{ 
public: 
	RequestMsg();
	virtual ~RequestMsg();
	static const char* strFingerprint;
	RpcHeader	header;
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
	RpcHeader	header;
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
