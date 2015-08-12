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
#include <vector>
class IBinProtocol: public IProtocol
{
public: 
	IBinProtocol (int8* buf,uint32 len):len_(len),curLen_(0),buf_(buf){}
	~IBinProtocol(){}

	virtual bool writeMsgBegin() {return true; }
	virtual bool writeMsgEnd()   { return true; }

	bool write(int8* data,uint32 len)
	{
		if (len==0)
			return true;
		if((len_-curLen_)>=len)
		{
			memcpy(buf_+curLen_,(int8*)data,len);
			curLen_+=len;
			return true;
		}
		return false;
	}

	bool read(int8* data,uint32 len)
	{
		if (len==0)
			return true;
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
	template <typename T>
	bool writeVector(std::vector<T>& vIn)
	{
		if (0==len_)
			return true;
		if (0== vIn.size())
			return true;
		writeUInt16(vIn.size());
		for (size_t i=0;i<vIn.size();i++)
		{
			if (curLen_>len_)
				return false;
			vIn[i].serialize(this);
		}
		return true;
	}
	template <typename T>
	bool readVector(std::vector<T>& vOut)
	{
		if (0==len_) return true;
		uint16 size=0;
		readUInt16(size);
		if (0==size) return true;
		vOut.resize(size);
		for(uint16 i=0;i<size;i++)
		{
			vOut[i].deSerialize(this);
		}
		return true;
	}
	bool writeVector(std::vector<int8>& vIn)
	{
		if (0==len_)
			return true;
		if (0== vIn.size())
			return true;
		writeUInt16(vIn.size());

		for (size_t i=0;i<vIn.size();i++)
		{
			if (curLen_>len_)
				return false;
			this->writeInt8(vIn[i]);
		}
		return true;
	}
	bool readVector(std::vector<int8>& vOut)
	{
		if (0==len_) return true;
		uint16 size=0;
		readUInt16(size);
		if (0==size) return true;
		vOut.resize(size);
		for(uint16 i=0;i<size;i++)
		{
			readInt8(vOut[i]);
		}
		return true;
	}
	bool writeVector(std::vector<int16>& vIn)
	{
		if (0==len_)
			return true;
		if (0== vIn.size())
			return true;
		writeUInt16(vIn.size());

		for (size_t i=0;i<vIn.size();i++)
		{
			if (curLen_>len_)
				return false;
			this->writeInt16(vIn[i]);
		}
		return true;
	}
	bool readVector(std::vector<int16>& vOut)
	{
		if (0==len_) return true;
		uint16 size=0;
		readUInt16(size);
		if (0==size) return true;
		vOut.resize(size);
		for(uint16 i=0;i<size;i++)
		{
			readInt16(vOut[i]);
		}
		return true;
	}

	bool writeVector(std::vector<int64>& vIn)
	{
		if (0==len_)
			return true;
		if (0== vIn.size())
			return true;
		writeUInt16(vIn.size());

		for (size_t i=0;i<vIn.size();i++)
		{
			if (curLen_>len_)
				return false;
			this->writeInt64(vIn[i]);
		}
		return true;
	}
	bool readVector(std::vector<int64>& vOut)
	{
		if (0==len_) return true;
		uint16 size=0;
		readUInt16(size);
		if (0==size) return true;
		vOut.resize(size);
		for(uint16 i=0;i<size;i++)
		{
			readInt64(vOut[i]);
		}
		return true;
	}

	uint32 getCurLen() { return curLen_; }

private:
	uint32 len_;		//<buf最大长度
	uint32 curLen_;		//<当前长度
	int8*  buf_;		//<数据
};

#endif
