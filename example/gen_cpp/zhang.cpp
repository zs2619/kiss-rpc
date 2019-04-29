#include"zhang.h"
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
		}
		default:
		{
			return false;
		}
	}//switch
}//dispatch
