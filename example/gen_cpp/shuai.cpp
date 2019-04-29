#include"shuai.h"
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

