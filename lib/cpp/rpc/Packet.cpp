#include"Packet.h"
void MsgProtocolTypeInit(rpc::EnumMap* e)
{ 
	e->addEnumValue("MPT_Bin");
	e->addEnumValue("MPT_Json");
} 
rpc::EnumMap RpcEnum(MsgProtocolType)(MsgProtocolTypeInit);

const char* rpc::RpcHeader::strFingerprint="28310e0dc8ad86ce46bf42a5037ee8d6";
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
	__P__->writeUInt8(version);

	__P__->writeInt16((rpc::int16)msgType);

	__P__->writeString(serviceName);

}// serialize

//deSerialize
bool rpc::RpcHeader::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!__P__->readUInt8(version))return false;

	rpc::int16  __temp_enum__msgType =0;
	if(!__P__->readInt16((rpc::int16&) __temp_enum__msgType))return false;
	msgType=(MsgProtocolType) __temp_enum__msgType;

	if(!__P__->readString(serviceName))return false;

	return true; 
}//deSerialize 

const char* rpc::RequestMsg::strFingerprint="2017ec81ad4621da5e3b8bd32bc985fe";
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

	__P__->writeUInt16(msgId);

	__P__->writeUInt16(rpc::uint16(buff.size()));
	for (size_t _i_buff_=0;_i_buff_<buff.size();_i_buff_++)
	{
		__P__->writeInt8(buff[_i_buff_]);
	}

}// serialize

//deSerialize
bool rpc::RequestMsg::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!header.deSerialize(__P__))return false;

	if(!__P__->readInt64(msgSeqId))return false;

	if(!__P__->readUInt16(msgId))return false;

	rpc::uint16 _n_buff_array=0;
	if(!__P__->readUInt16(_n_buff_array))return false;
	buff.resize( _n_buff_array);
	for (size_t _i_buff_=0;_i_buff_<_n_buff_array;_i_buff_++)
	{
		if(!__P__->readInt8(buff[_i_buff_]))return false;
	}

	return true; 
}//deSerialize 

const char* rpc::ResponseMsg::strFingerprint="ac24ea21771056228cb098ed462458e0";
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

	__P__->writeUInt16(msgId);

	__P__->writeUInt16(rpc::uint16(buff.size()));
	for (size_t _i_buff_=0;_i_buff_<buff.size();_i_buff_++)
	{
		__P__->writeInt8(buff[_i_buff_]);
	}

}// serialize

//deSerialize
bool rpc::ResponseMsg::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!header.deSerialize(__P__))return false;

	if(!__P__->readInt64(msgSeqId))return false;

	if(!__P__->readUInt16(msgId))return false;

	rpc::uint16 _n_buff_array=0;
	if(!__P__->readUInt16(_n_buff_array))return false;
	buff.resize( _n_buff_array);
	for (size_t _i_buff_=0;_i_buff_<_n_buff_array;_i_buff_++)
	{
		if(!__P__->readInt8(buff[_i_buff_]))return false;
	}

	return true; 
}//deSerialize 

