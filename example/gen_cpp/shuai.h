#ifndef	__SHUAI_H__
#define	__SHUAI_H__

#include <functional>
#include "rpc/Common.h"
#include "rpc/Protocol.h"
namespace shuai{

enum SexType
{ 
	male,
	famale,
}; 
extern rpc::EnumMap RpcEnum(SexType);

class role
{ 
public: 
	role();
	virtual ~role();
	static const char* strFingerprint;
	SexType	sexType;
	bool	b;
	rpc::byte	by;
	rpc::int8	i8;
	rpc::int16	i16;
	rpc::int32	i32;
	rpc::int64	i64;
	float	f;
	std::string	str;
	std::vector<byte>	bin;

	//serialize
	void serialize(rpc::IProtocol* __P__) const ;

	//deSerialize
	bool deSerialize(rpc::IProtocol* __P__);
} ;//struct

class user
{ 
public: 
	user();
	virtual ~user();
	static const char* strFingerprint;
	std::map<rpc::int32,role>	roleMap;
	std::vector<rpc::int32> 	items;
	std::vector<role> 	roles;

	//serialize
	void serialize(rpc::IProtocol* __P__) const ;

	//deSerialize
	bool deSerialize(rpc::IProtocol* __P__);
} ;//struct

}///namespace
#endif
