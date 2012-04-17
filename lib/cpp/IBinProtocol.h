//==============================================
/**
	@date:		2012:4:16  
	@file: 		IBinProtocol.h
	@author		zhangshuai
*/
//==============================================

#ifndef	__IBINPROTOCOL_H__
#define	__IBINPROTOCOL_H__
#include "IProtocol.h"
#include "Common.h"
class IBinProtocol: public IProtocol
{
public: 
	IBinProtocol (int8* buf,uint32 len):buf_(buf),len_(len),curLen_(0){}
	~IBinProtocol(){}

	virtual bool writeMsgBegin() {return true; }
	virtual bool writeMsgEnd()   { return true; }

	bool write(int8* data,uint32 len)
	{
		if((len_-curLen_)>=len)
		{
			memcpy(buf_+curLen_,(int8*)&data,len);
			curLen_+=len;
			return true;
		}
		return false;
	}

	bool read(int8* data,uint32 len)
	{
		if((len_-curLen_)>=len)
		{
			memcpy(data,buf_+curLen_,len);
			curLen_+=len;
			return true;
		}
		return false;
	}
	virtual bool readString(std::string& str)
	{
		uint16	len=0;
		if (read((int8*)&len,2))
		{
			str=std::string((char*)buf_+curLen_,len);
			curLen_+=len;
			return true;
		}
		return false;
	}
	uint32 getCurLen() { return curLen_; }

private:
	uint32 len_;		//<buf最大长度
	uint32 curLen_;		//<当前长度
	int8*  buf_;		//<数据
};

#endif