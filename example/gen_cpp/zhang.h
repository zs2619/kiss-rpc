#ifndef	__ZHANG_H__
#define	__ZHANG_H__

#include <functional>
#include "rpc/Common.h"
#include "rpc/Protocol.h"
#include "rpc/ServiceStub.h"
#include "rpc/ServiceProxy.h"
#include "shuai.h"
namespace shuai{

class opServiceStub: public rpc::ServiceStub
{ 
public: 
	static const char* strFingerprint;
	static const char* getObjName;
	enum {
		login_Id,
		xixi_Id,
		lala_Id,
		ping_Id,
	};
	opServiceStub(const rpc::Connection* conn):ServiceStub(conn){}
	virtual ~opServiceStub(){}
	void invokeAsync(rpc::int16 msgId,const rpc::IProtocol* p,const std::string& functionName);
	virtual bool dispatch(std::shared_ptr<rpc::RpcMsg> msg);
	void login(const std::string&  openid,std::function<int(rpc::int8)> cb);
	void xixi(user&  u,std::function<int(user)> cb);
	void lala(std::map<rpc::int32,role>&  m,std::vector<rpc::int32> &  ai,std::vector<role> &  ar,std::function<int(std::vector<role> )> cb);
	void ping(rpc::int8  i8,std::function<int(rpc::int8)> cb);

protected:
	std::function<int(rpc::int8)> loginCallBack;
	std::function<int(user)> xixiCallBack;
	std::function<int(std::vector<role> )> lalaCallBack;
	std::function<int(rpc::int8)> pingCallBack;
};//class

class opServiceProxyIF: public rpc::ServiceProxy 
{ 
public: 
	static const char* strFingerprint;
	static const char* getObjName;
	opServiceProxyIF(const rpc::Connection* conn):ServiceProxy(conn){}
	virtual ~opServiceProxyIF(){}
	virtual std::tuple<int,rpc::int8>login(const std::string&  openid)=0;
	virtual std::tuple<int,user>xixi(user&  u)=0;
	virtual std::tuple<int,std::vector<role> >lala(std::map<rpc::int32,role>&  m,std::vector<rpc::int32> &  ai,std::vector<role> &  ar)=0;
	virtual std::tuple<int,rpc::int8>ping(rpc::int8  i8)=0;
	virtual bool dispatch(std::shared_ptr<rpc::RpcMsg> msg);

};//class

}///namespace
#endif
