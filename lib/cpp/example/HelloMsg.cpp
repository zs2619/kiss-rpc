#include"HelloMsg.h"
void SexTypeInit(rpc::EnumMap* e)
{ 
	e->addEnumValue("male");
	e->addEnumValue("famale");
} 
rpc::EnumMap RpcEnum(SexType)(SexTypeInit);

const char* shuai::role::strFingerprint="5ad2cb5c4c22f48680d17d2a084021b2";
shuai::role::role()
:sexType(SexType(0))
,b(false)
,by(0)
,i8(0)
,i16(0)
,i32(0)
,i64(0)
,f(0.0)
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

	__P__->writeByte(by);

	__P__->writeInt8(i8);

	__P__->writeInt16(i16);

	__P__->writeInt32(i32);

	__P__->writeInt64(i64);

	__P__->writeFloat(f);

	__P__->writeString(str);

	__P__->writeBinary(bin);

}// serialize

//deSerialize
bool shuai::role::deSerialize(rpc::IProtocol* __P__)
{ 
	rpc::int16  __temp_enum__sexType =0;
	if(!__P__->readInt16((rpc::int16&) __temp_enum__sexType))return false;
	sexType=(SexType) __temp_enum__sexType;

	if(!__P__->readBool(b))return false;

	if(!__P__->readByte(by))return false;

	if(!__P__->readInt8(i8))return false;

	if(!__P__->readInt16(i16))return false;

	if(!__P__->readInt32(i32))return false;

	if(!__P__->readInt64(i64))return false;

	if(!__P__->readFloat(f))return false;

	if(!__P__->readString(str))return false;

	if(!__P__->readBinary(bin))return false;

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
	__P__->writeInt32(rpc::int32(roleMap.size()));
	std::map<rpc::int32,role>::const_iterator _it_roleMap_ = roleMap.begin();
	while(_it_roleMap_!=roleMap.end())
	{
		__P__->writeInt32(_it_roleMap_->first);
		_it_roleMap_->second.serialize(__P__);
		++_it_roleMap_;
	}

	__P__->writeInt32(rpc::int32(items.size()));
	for (size_t _i_items_=0;_i_items_<items.size();_i_items_++)
	{
		__P__->writeInt32(items[_i_items_]);
	}

	__P__->writeInt32(rpc::int32(roles.size()));
	for (size_t _i_roles_=0;_i_roles_<roles.size();_i_roles_++)
	{
		roles[_i_roles_].serialize(__P__);
	}

}// serialize

//deSerialize
bool shuai::user::deSerialize(rpc::IProtocol* __P__)
{ 
	rpc::int32 _n_roleMap_map_=0;
	if(!__P__->readInt32(_n_roleMap_map_))return false;
	for (rpc::int32 _i_roleMap_=0;_i_roleMap_<_n_roleMap_map_;_i_roleMap_++)
	{
		rpc::int32  _first_map_roleMap;
		role  _second_map_roleMap;
		if(!__P__->readInt32( _first_map_roleMap))return false;
		if(! _second_map_roleMap.deSerialize(__P__))return false;
		roleMap[ _first_map_roleMap]= _second_map_roleMap;
	}

	rpc::int32 _n_items_array=0;
	if(!__P__->readInt32(_n_items_array))return false;
	items.resize( _n_items_array);
	for (rpc::int32 _i_items_=0;_i_items_<_n_items_array;_i_items_++)
	{
		if(!__P__->readInt32(items[_i_items_]))return false;
	}

	rpc::int32 _n_roles_array=0;
	if(!__P__->readInt32(_n_roles_array))return false;
	roles.resize( _n_roles_array);
	for (rpc::int32 _i_roles_=0;_i_roles_<_n_roles_array;_i_roles_++)
	{
		if(!roles[_i_roles_].deSerialize(__P__))return false;
	}

	return true; 
}//deSerialize 

const char* shuai::opServiceStub::strFingerprint="878e3bfc527601565994f11bb9d347d0";
const char* shuai::opServiceStub::getObjName="shuai.opService";
void  shuai::opServiceStub::invokeAsync(rpc::int16 msgId,const rpc::IProtocol* p ,const std::string& functionName) {
	std::shared_ptr<rpc::RpcMsg> msg = std::make_shared<rpc::RpcMsg>();
	msg->serviceName_= getObjName ;
	msg->functionName_= functionName ;
	msg->requestMsg_.msgId = msgId;
	msg->requestMsg_.buff = p->getBuffer();
	invoke(msg);
}
bool  shuai::opServiceStub::dispatch(std::shared_ptr<rpc::RpcMsg> msg) {
	auto id=msg->responseMsg_.msgId;
	switch (id)
	{
		case 0:
		{
			std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			rpc::int8 ret ;
			if(!__P__.get()->readInt8(ret))return false;
			int cbRet = loginCallBack(ret);
			return true;
		}
		case 1:
		{
			std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			user ret ;
			if(!ret.deSerialize(__P__.get()))return false;
			int cbRet = xixiCallBack(ret);
			return true;
		}
		case 2:
		{
			std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->responseMsg_.buff);
			std::vector<role>  ret ;
			rpc::int32 _n_ret_array=0;
			if(!__P__.get()->readInt32(_n_ret_array))return false;
			ret.resize( _n_ret_array);
			for (rpc::int32 _i_ret_=0;_i_ret_<_n_ret_array;_i_ret_++)
			{
				if(!ret[_i_ret_].deSerialize(__P__.get()))return false;
			}
			int cbRet = lalaCallBack(ret);
			return true;
		}
		case 3:
		{
			std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
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
void shuai::opServiceStub::login(const std::string&  openid,std::function<int(rpc::int8)> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
	__P__.get()->writeString(openid);

	loginCallBack = cb;
	invokeAsync(0,__P__.get(),"login");
}
void shuai::opServiceStub::xixi(user&  u,std::function<int(user)> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
	u.serialize(__P__.get());

	xixiCallBack = cb;
	invokeAsync(1,__P__.get(),"xixi");
}
void shuai::opServiceStub::lala(std::map<rpc::int32,role>&  m,std::vector<rpc::int32> &  ai,std::vector<role> &  ar,std::function<int(std::vector<role> )> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
	__P__.get()->writeInt32(rpc::int32(m.size()));
	std::map<rpc::int32,role>::const_iterator _it_m_ = m.begin();
	while(_it_m_!=m.end())
	{
		__P__.get()->writeInt32(_it_m_->first);
		_it_m_->second.serialize(__P__.get());
		++_it_m_;
	}

	__P__.get()->writeInt32(rpc::int32(ai.size()));
	for (size_t _i_ai_=0;_i_ai_<ai.size();_i_ai_++)
	{
		__P__.get()->writeInt32(ai[_i_ai_]);
	}

	__P__.get()->writeInt32(rpc::int32(ar.size()));
	for (size_t _i_ar_=0;_i_ar_<ar.size();_i_ar_++)
	{
		ar[_i_ar_].serialize(__P__.get());
	}

	lalaCallBack = cb;
	invokeAsync(2,__P__.get(),"lala");
}
void shuai::opServiceStub::ping(rpc::int8  i8,std::function<int(rpc::int8)> cb)
{
	std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());
	__P__.get()->writeInt8(i8);

	pingCallBack = cb;
	invokeAsync(3,__P__.get(),"ping");
}
const char* shuai::opServiceProxyIF::strFingerprint="878e3bfc527601565994f11bb9d347d0";
const char* shuai::opServiceProxyIF::getObjName="shuai.opService";
bool shuai::opServiceProxyIF::dispatch(std::shared_ptr<rpc::RpcMsg> msg)
{
	auto id=msg->requestMsg_.msgId;
	msg->serviceName_=getObjName;
	switch (id)
	{
		case  0 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			std::string openid;
			if(!__P__.get()->readString(openid))return false;

			auto result = login(openid);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
				rpc::int8 ret=std::get<1>(result);
				__P__.get()->writeInt8(ret);
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
			return true;
		}
		case  1 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			user u;
			if(!u.deSerialize(__P__.get()))return false;

			auto result = xixi(u);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
				user ret=std::get<1>(result);
				ret.serialize(__P__.get());
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
			return true;
		}
		case  2 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			std::map<rpc::int32,role> m;
			rpc::int32 _n_m_map_=0;
			if(!__P__.get()->readInt32(_n_m_map_))return false;
			for (rpc::int32 _i_m_=0;_i_m_<_n_m_map_;_i_m_++)
			{
				rpc::int32  _first_map_m;
				role  _second_map_m;
				if(!__P__.get()->readInt32( _first_map_m))return false;
				if(! _second_map_m.deSerialize(__P__.get()))return false;
				m[ _first_map_m]= _second_map_m;
			}

			std::vector<rpc::int32>  ai;
			rpc::int32 _n_ai_array=0;
			if(!__P__.get()->readInt32(_n_ai_array))return false;
			ai.resize( _n_ai_array);
			for (rpc::int32 _i_ai_=0;_i_ai_<_n_ai_array;_i_ai_++)
			{
				if(!__P__.get()->readInt32(ai[_i_ai_]))return false;
			}

			std::vector<role>  ar;
			rpc::int32 _n_ar_array=0;
			if(!__P__.get()->readInt32(_n_ar_array))return false;
			ar.resize( _n_ar_array);
			for (rpc::int32 _i_ar_=0;_i_ar_<_n_ar_array;_i_ar_++)
			{
				if(!ar[_i_ar_].deSerialize(__P__.get()))return false;
			}

			auto result = lala(m,ai,ar);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
				std::vector<role>  ret=std::get<1>(result);
				__P__.get()->writeInt32(rpc::int32(ret.size()));
				for (size_t _i_ret_=0;_i_ret_<ret.size();_i_ret_++)
				{
					ret[_i_ret_].serialize(__P__.get());
				}
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
			return true;
		}
		case  3 :
		{
			std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
			__P__->setBuffer(msg->requestMsg_.buff);
			rpc::int8 i8;
			if(!__P__.get()->readInt8(i8))return false;

			auto result = ping(i8);
			if (std::get<0>(result)==0)
			{
				std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());
				rpc::int8 ret=std::get<1>(result);
				__P__.get()->writeInt8(ret);
				msg->responseMsg_.buff=__P__->getBuffer();
				invoke(msg);
			}
			return true;
		}
		default:
		{
			return false;
		}
	}//switch
}//dispatch
