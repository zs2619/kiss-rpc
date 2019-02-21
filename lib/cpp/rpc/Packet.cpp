#include"Packet.h"
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

	__P__->writeUInt16(rpc::uint16(buff.size()));
	for (size_t _i_buff_=0;_i_buff_<buff.size();_i_buff_++)
	{
		__P__->writeInt8(buff[_i_buff_]);
	}

}// serialize

//deSerialize
bool rpc::RequestMsg::deSerialize(rpc::IProtocol* __P__)
{ 
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

	__P__->writeUInt16(rpc::uint16(buff.size()));
	for (size_t _i_buff_=0;_i_buff_<buff.size();_i_buff_++)
	{
		__P__->writeInt8(buff[_i_buff_]);
	}

}// serialize

//deSerialize
bool rpc::ResponseMsg::deSerialize(rpc::IProtocol* __P__)
{ 
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

