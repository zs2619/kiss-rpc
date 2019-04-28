#include"Packet.h"
void MsgProtocolTypeInit(rpc::EnumMap* e)
{ 
	e->addEnumValue("MPT_Bin");
	e->addEnumValue("MPT_Json");
} 
rpc::EnumMap RpcEnum(MsgProtocolType)(MsgProtocolTypeInit);

const char* rpc::RpcHeader::strFingerprint="2bcc93ddec2a7ac9beece683a78b5561";
rpc::RpcHeader::RpcHeader()
:version(0)
,msgType(MsgProtocolType(0))
{ 
} 
rpc::RpcHeader::~RpcHeader()
{ 
} 

//serialize
void rpc::RpcHeader::serialize(rpc::IProtocol* __P__) const 
{ 
	__P__->writeInt8(version);

	__P__->writeInt16((rpc::int16)msgType);

	__P__->writeString(serviceName);

}// serialize

//deSerialize
bool rpc::RpcHeader::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!__P__->readInt8(version))return false;

	rpc::int16  __temp_enum__msgType =0;
	if(!__P__->readInt16((rpc::int16&) __temp_enum__msgType))return false;
	msgType=(MsgProtocolType) __temp_enum__msgType;

	if(!__P__->readString(serviceName))return false;

	return true; 
}//deSerialize 

const char* rpc::RequestMsg::strFingerprint="e61c37380db853addbaafae6c4850b2e";
rpc::RequestMsg::RequestMsg()
:msgSeqId(0)
,msgId(0)
{ 
} 
rpc::RequestMsg::~RequestMsg()
{ 
} 

//serialize
void rpc::RequestMsg::serialize(rpc::IProtocol* __P__) const 
{ 
	header.serialize(__P__);

	__P__->writeInt64(msgSeqId);

	__P__->writeInt32(msgId);

	__P__->writeBinary(buff);

}// serialize

//deSerialize
bool rpc::RequestMsg::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!header.deSerialize(__P__))return false;

	if(!__P__->readInt64(msgSeqId))return false;

	if(!__P__->readInt32(msgId))return false;

	if(!__P__->readBinary(buff))return false;

	return true; 
}//deSerialize 

const char* rpc::ResponseMsg::strFingerprint="d44e3caf8903d1f381a22194a181c1cc";
rpc::ResponseMsg::ResponseMsg()
:msgSeqId(0)
,msgId(0)
{ 
} 
rpc::ResponseMsg::~ResponseMsg()
{ 
} 

//serialize
void rpc::ResponseMsg::serialize(rpc::IProtocol* __P__) const 
{ 
	header.serialize(__P__);

	__P__->writeInt64(msgSeqId);

	__P__->writeInt32(msgId);

	__P__->writeBinary(buff);

}// serialize

//deSerialize
bool rpc::ResponseMsg::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!header.deSerialize(__P__))return false;

	if(!__P__->readInt64(msgSeqId))return false;

	if(!__P__->readInt32(msgId))return false;

	if(!__P__->readBinary(buff))return false;

	return true; 
}//deSerialize 

