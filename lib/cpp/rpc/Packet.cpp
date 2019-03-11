#include"Packet.h"
void MsgProtocolTypeInit(rpc::EnumMap* e)
{ 
	e->addEnumValue("MPT_Bin");
	e->addEnumValue("MPT_Json");
} 
rpc::EnumMap RpcEnum(MsgProtocolType)(MsgProtocolTypeInit);

const char* rpc::RpcHeader::strFingerprint="04d60b1c355b04281ee50bf1070fb772";
rpc::RpcHeader::RpcHeader()
:version(0)
,msgType(MsgProtocolType(0))
,bodyLen(0)
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

	__P__->writeUInt16(bodyLen);

}// serialize

//deSerialize
bool rpc::RpcHeader::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!__P__->readUInt8(version))return false;

	rpc::int16  __temp_enum__msgType =0;
	if(!__P__->readInt16((rpc::int16&) __temp_enum__msgType))return false;
	msgType=(MsgProtocolType) __temp_enum__msgType;

	if(!__P__->readString(serviceName))return false;

	if(!__P__->readUInt16(bodyLen))return false;

	return true; 
}//deSerialize 

const char* rpc::RequestMsg::strFingerprint="97784b165bc31f74cc021909d44da108";
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
	__P__->writeInt64(msgSeqId);

	__P__->writeUInt16(msgId);

	__P__->writeUInt16(rpc::uint16(buf.size()));
	for (size_t _i_buf_=0;_i_buf_<buf.size();_i_buf_++)
	{
		__P__->writeInt8(buf[_i_buf_]);
	}

}// serialize

//deSerialize
bool rpc::RequestMsg::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!__P__->readInt64(msgSeqId))return false;

	if(!__P__->readUInt16(msgId))return false;

	rpc::uint16 _n_buf_array=0;
	if(!__P__->readUInt16(_n_buf_array))return false;
	buf.resize( _n_buf_array);
	for (size_t _i_buf_=0;_i_buf_<_n_buf_array;_i_buf_++)
	{
		if(!__P__->readInt8(buf[_i_buf_]))return false;
	}

	return true; 
}//deSerialize 

const char* rpc::ResponseMsg::strFingerprint="828608596d841596d1cf25c96c0b59e3";
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
	__P__->writeInt64(msgSeqId);

	__P__->writeUInt16(msgId);

	__P__->writeUInt16(rpc::uint16(buf.size()));
	for (size_t _i_buf_=0;_i_buf_<buf.size();_i_buf_++)
	{
		__P__->writeInt8(buf[_i_buf_]);
	}

}// serialize

//deSerialize
bool rpc::ResponseMsg::deSerialize(rpc::IProtocol* __P__)
{ 
	if(!__P__->readInt64(msgSeqId))return false;

	if(!__P__->readUInt16(msgId))return false;

	rpc::uint16 _n_buf_array=0;
	if(!__P__->readUInt16(_n_buf_array))return false;
	buf.resize( _n_buf_array);
	for (size_t _i_buf_=0;_i_buf_<_n_buf_array;_i_buf_++)
	{
		if(!__P__->readInt8(buf[_i_buf_]))return false;
	}

	return true; 
}//deSerialize 

