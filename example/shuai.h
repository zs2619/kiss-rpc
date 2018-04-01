#ifndef	__shuai_H__
#define	__shuai_H__

#include "IProtocol.h"
#include "Common.h"

struct user
{ 
	user();
	virtual ~user();
	static const char* strFingerprint;
	bool	b;
	int8	i;

	//serialize
	void serialize(IProtocol* __P__); 

	//deSerialize
	bool deSerialize(IProtocol* __P__);
} ;//struct

struct fdfa
{ 
	fdfa();
	virtual ~fdfa();
	static const char* strFingerprint;
	int8	f;
	user	u;
	std::vector<int16> 	fdf;

	//serialize
	void serialize(IProtocol* __P__); 

	//deSerialize
	bool deSerialize(IProtocol* __P__);
} ;//struct

class shaStub
{ 
public: 
	static const char* strFingerprint;
	enum {
		ping_Id,
	};
	shaStub(IProtocol* p=NULL):__P__(p){}
	virtual ~shaStub(){}
	void ping();
	IProtocol* __P__;
};//class

class shaProxy
{ 
public: 
	static const char* strFingerprint;
	shaProxy(IProtocol* p=NULL):__P__(p){}
	virtual ~shaProxy(){}
	virtual bool ping()=0;

	static bool recv_ping(IProtocol* p,shaProxy* __C__);
	static bool dispatch(IProtocol* p,shaProxy* __C__);

	IProtocol* __P__;
};//class

#endif
