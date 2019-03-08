#include"HelloMsg.h"
void SexTypeInit(rpc::EnumMap* e)
{ 
	e->addEnumValue("male");
	e->addEnumValue("famale");
} 
rpc::EnumMap RpcEnum(SexType)(SexTypeInit);

const char* shuai::role::strFingerprint="8163856412fcd3050d832f677d6ff6a0";
shuai::role::role()
:sexType(SexType(0))
,b(false)
,i8(0)
,ui8(0)
,i16(0)
,ui16(0)
,i32(0)
,ui32(0)
,i64(0)
{ 
} 
shuai::role::~role()
{ 
} 

//serialize
void shuai::role::serialize(rpc::IProtocol* __P__) const 
{ 
	__P__->writeInt16((rpc::int16)sexType);

	__P__->writeBool(b);

	__P__->writeInt8(i8);

	__P__->writeUInt8(ui8);

	__P__->writeInt16(i16);

	__P__->writeUInt16(ui16);

	__P__->writeInt32(i32);

	__P__->writeUInt32(ui32);

	__P__->writeInt64(i64);

	__P__->writeString(str);

}// serialize

//deSerialize
bool shuai::role::deSerialize(rpc::IProtocol* __P__)
{ 
	rpc::int16  __temp_enum__sexType =0;
	if(!__P__->readInt16((rpc::int16&) __temp_enum__sexType))return false;
	sexType=(SexType) __temp_enum__sexType;

	if(!__P__->readBool(b))return false;

	if(!__P__->readInt8(i8))return false;

	if(!__P__->readUInt8(ui8))return false;

	if(!__P__->readInt16(i16))return false;

	if(!__P__->readUInt16(ui16))return false;

	if(!__P__->readInt32(i32))return false;

	if(!__P__->readUInt32(ui32))return false;

	if(!__P__->readInt64(i64))return false;

	if(!__P__->readString(str))return false;

	return true; 
}//deSerialize 

const char* shuai::user::strFingerprint="58d411b316d3ad09ca4c27108e0ca7d7";
shuai::user::user()
{ 
} 
shuai::user::~user()
{ 
} 

//serialize
void shuai::user::serialize(rpc::IProtocol* __P__) const 
{ 
	__P__->writeUInt16(rpc::uint16(roleMap.size()));
	std::map<rpc::int32,role>::const_iterator _it_roleMap_ = roleMap.begin();
	while(_it_roleMap_!=roleMap.end())
	{
		__P__->writeInt32(_it_roleMap_->first);
		_it_roleMap_->second.serialize(__P__);
		++_it_roleMap_;
	}

	__P__->writeUInt16(rpc::uint16(items.size()));
	for (size_t _i_items_=0;_i_items_<items.size();_i_items_++)
	{
		__P__->writeInt32(items[_i_items_]);
	}

	__P__->writeUInt16(rpc::uint16(roles.size()));
	for (size_t _i_roles_=0;_i_roles_<roles.size();_i_roles_++)
	{
		roles[_i_roles_].serialize(__P__);
	}

}// serialize

//deSerialize
bool shuai::user::deSerialize(rpc::IProtocol* __P__)
{ 
	rpc::uint16 _n_roleMap_map_=0;
	if(!__P__->readUInt16(_n_roleMap_map_))return false;
	for (int _i_roleMap_=0;_i_roleMap_<_n_roleMap_map_;_i_roleMap_++)
	{
		rpc::int32  _first_map_roleMap;
		role  _second_map_roleMap;
		if(!__P__->readInt32( _first_map_roleMap))return false;
		if(! _second_map_roleMap.deSerialize(__P__))return false;
		roleMap[ _first_map_roleMap]= _second_map_roleMap;
	}

	rpc::uint16 _n_items_array=0;
	if(!__P__->readUInt16(_n_items_array))return false;
	items.resize( _n_items_array);
	for (size_t _i_items_=0;_i_items_<_n_items_array;_i_items_++)
	{
		if(!__P__->readInt32(items[_i_items_]))return false;
	}

	rpc::uint16 _n_roles_array=0;
	if(!__P__->readUInt16(_n_roles_array))return false;
	roles.resize( _n_roles_array);
	for (size_t _i_roles_=0;_i_roles_<_n_roles_array;_i_roles_++)
	{
		if(!roles[_i_roles_].deSerialize(__P__))return false;
	}

	return true; 
}//deSerialize 

const char* shuai::opServiceStub::strFingerprint="e5212fe49d9deba0e43271889ff23ec0";
const char* shuai::opServiceStub::getObjName="shuai.opService.stub";
void  shuai::opServiceStub::invokeAsync(rpc::uint16 msgId,const rpc::IProtocol* p) {
	std::shared_ptr<rpc::RpcMsg> msg = std::make_shared<rpc::RpcMsg>();
	msg->requestMsg_.msgId = msgId;
	msg->requestMsg_.buff = p->getBuffer();
	invoke(msg);
}
bool  shuai::opServiceStub::dispatch(std::shared_ptr<rpc::RpcMsg> msg) {
	auto id=msg->responseMsg_.msgId;
	switch (id)
	{
		case 1:
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			rpc::int8 ret ;
			if(!__P__.get()->readInt8(ret))return false;
			int cbRet = testCallBack(ret);
			return true;
		}
		case 2:
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			rpc::int8 ret ;
			if(!__P__.get()->readInt8(ret))return false;
			int cbRet = xixiCallBack(ret);
			return true;
		}
		case 3:
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			std::vector<role>  ret ;
			rpc::uint16 _n_ret_array=0;
			if(!__P__.get()->readUInt16(_n_ret_array))return false;
			ret.resize( _n_ret_array);
			for (size_t _i_ret_=0;_i_ret_<_n_ret_array;_i_ret_++)
			{
				if(!ret[_i_ret_].deSerialize(__P__.get()))return false;
			}
			int cbRet = lalaCallBack(ret);
			return true;
		}
		case 4:
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			rpc::int8 ret ;
			if(!__P__.get()->readInt8(ret))return false;
			int cbRet = pingCallBack(ret);
			return true;
		}
		default:
		{
			return false;
		}
	}
}
void shuai::opServiceStub::momo(rpc::int8  i8,rpc::int64  i64)
{
	std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
	__P__.get()->writeInt8(i8);

	__P__.get()->writeInt64(i64);

	invokeAsync(0,__P__.get());
}
void shuai::opServiceStub::test(rpc::int8  i8,std::function<int(rpc::int8)> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
	__P__.get()->writeInt8(i8);

	testCallBack = cb;
	invokeAsync(1,__P__.get());
}
void shuai::opServiceStub::xixi(user&  u,std::function<int(rpc::int8)> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
	u.serialize(__P__.get());

	xixiCallBack = cb;
	invokeAsync(2,__P__.get());
}
void shuai::opServiceStub::lala(std::map<rpc::int32,role>&  m,std::vector<rpc::int32> &  ai,std::vector<role> &  ar,std::function<int(std::vector<role> )> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
	__P__.get()->writeUInt16(rpc::uint16(m.size()));
	std::map<rpc::int32,role>::const_iterator _it_m_ = m.begin();
	while(_it_m_!=m.end())
	{
		__P__.get()->writeInt32(_it_m_->first);
		_it_m_->second.serialize(__P__.get());
		++_it_m_;
	}

	__P__.get()->writeUInt16(rpc::uint16(ai.size()));
	for (size_t _i_ai_=0;_i_ai_<ai.size();_i_ai_++)
	{
		__P__.get()->writeInt32(ai[_i_ai_]);
	}

	__P__.get()->writeUInt16(rpc::uint16(ar.size()));
	for (size_t _i_ar_=0;_i_ar_<ar.size();_i_ar_++)
	{
		ar[_i_ar_].serialize(__P__.get());
	}

	lalaCallBack = cb;
	invokeAsync(3,__P__.get());
}
void shuai::opServiceStub::ping(std::function<int(rpc::int8)> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
	pingCallBack = cb;
	invokeAsync(4,__P__.get());
}
const char* shuai::opServiceProxyIF::strFingerprint="e5212fe49d9deba0e43271889ff23ec0";
const char* shuai::opServiceProxyIF::getObjName="shuai.opService.proxy";
bool shuai::opServiceProxyIF::dispatch(std::shared_ptr<rpc::RpcMsg> msg)
{
	auto id=msg->requestMsg_.msgId;
	switch (id)
	{
		case  0 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			rpc::int8 i8;
			if(!__P__.get()->readInt8(i8))return false;

			rpc::int64 i64;
			if(!__P__.get()->readInt64(i64))return false;

			auto result = momo(i8,i64);
		}
		case  1 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			rpc::int8 i8;
			if(!__P__.get()->readInt8(i8))return false;

			auto result = test(i8);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
				rpc::int8 ret=std::get<1>(result);
				__P__.get()->writeInt8(ret);
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
		}
		case  2 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			user u;
			if(!u.deSerialize(__P__.get()))return false;

			auto result = xixi(u);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
				rpc::int8 ret=std::get<1>(result);
				__P__.get()->writeInt8(ret);
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
		}
		case  3 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			std::map<rpc::int32,role> m;
			rpc::uint16 _n_m_map_=0;
			if(!__P__.get()->readUInt16(_n_m_map_))return false;
			for (int _i_m_=0;_i_m_<_n_m_map_;_i_m_++)
			{
				rpc::int32  _first_map_m;
				role  _second_map_m;
				if(!__P__.get()->readInt32( _first_map_m))return false;
				if(! _second_map_m.deSerialize(__P__.get()))return false;
				m[ _first_map_m]= _second_map_m;
			}

			std::vector<rpc::int32>  ai;
			rpc::uint16 _n_ai_array=0;
			if(!__P__.get()->readUInt16(_n_ai_array))return false;
			ai.resize( _n_ai_array);
			for (size_t _i_ai_=0;_i_ai_<_n_ai_array;_i_ai_++)
			{
				if(!__P__.get()->readInt32(ai[_i_ai_]))return false;
			}

			std::vector<role>  ar;
			rpc::uint16 _n_ar_array=0;
			if(!__P__.get()->readUInt16(_n_ar_array))return false;
			ar.resize( _n_ar_array);
			for (size_t _i_ar_=0;_i_ar_<_n_ar_array;_i_ar_++)
			{
				if(!ar[_i_ar_].deSerialize(__P__.get()))return false;
			}

			auto result = lala(m,ai,ar);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
				std::vector<role>  ret=std::get<1>(result);
				__P__.get()->writeUInt16(rpc::uint16(ret.size()));
				for (size_t _i_ret_=0;_i_ret_<ret.size();_i_ret_++)
				{
					ret[_i_ret_].serialize(__P__.get());
				}
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
		}
		case  4 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			auto result = ping();
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(getProtocol()->createProtoBuffer());
				rpc::int8 ret=std::get<1>(result);
				__P__.get()->writeInt8(ret);
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
		}
		default:
		{
			return false;
		}
	}//switch
}//dispatch
