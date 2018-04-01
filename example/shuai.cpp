#include"shuai.h"
const char* user::strFingerprint="0209ef47d2899f1564700ea98a01d03a";
user::user()
:b(false)
,i(0)
{ 
} 
user::~user()
{ 
} 

//serialize
void user::serialize(IProtocol* __P__) 
{ 
	__P__->writeBool(b);

	__P__->writeInt8(i);

}// serialize

//deSerialize
bool user::deSerialize(IProtocol* __P__)
{ 
	if(!__P__->readBool(b))return false;

	if(!__P__->readInt8(i))return false;

	return true; 
}//deSerialize 

const char* fdfa::strFingerprint="3c78471af08353dd30b92f2debd851b8";
fdfa::fdfa()
:f(0)
{ 
} 
fdfa::~fdfa()
{ 
} 

//serialize
void fdfa::serialize(IProtocol* __P__) 
{ 
	__P__->writeInt8(f);

	u.serialize(__P__);

	__P__->writeUInt16(fdf.size());
	for (size_t _i_fdf_=0;_i_fdf_<fdf.size();_i_fdf_++)
	{
		__P__->writeInt16(fdf[_i_fdf_]);
	}

}// serialize

//deSerialize
bool fdfa::deSerialize(IProtocol* __P__)
{ 
	if(!__P__->readInt8(f))return false;

	if(!u.deSerialize(__P__))return false;

	uint16 _n_fdf_array=0;
	if(!__P__->readUInt16(_n_fdf_array))return false;
	fdf.resize( _n_fdf_array);
	for (size_t _i_fdf_=0;_i_fdf_<_n_fdf_array;_i_fdf_++)
	{
		if(!__P__->readInt16(fdf[_i_fdf_]))return false;
	}

	return true; 
}//deSerialize 

const char* shaStub::strFingerprint="3b3df70d055ca4d852187bff21d93eb1";
void shaStub::ping()
{
	__P__->writeMsgBegin();
	__P__->writeUInt16(0);
	__P__->writeMsgEnd();
}
const char* shaProxy::strFingerprint="3b3df70d055ca4d852187bff21d93eb1";
bool shaProxy::dispatch(IProtocol* __P__,shaProxy* __C__)
{
	uint16 id=0;
	if(!__P__->readUInt16(id)) return false;
	switch (id)
	{
		case  0 :
		{
			return recv_ping(__P__,__C__);
		}
		default:
		{
			return false;
		}
	}//switch
}//dispatch
bool shaProxy::recv_ping(IProtocol* __P__,shaProxy* __C__)
{
	return __C__->ping();
}
