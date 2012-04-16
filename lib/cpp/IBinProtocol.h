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

	virtual bool writeMsgBegin() { }
	virtual bool writeMsgEnd() { }

	virtual bool writeBool(bool value)
	{
		uint8 i=value?1:0;
		return write((int8*)&i,1);
	}

	virtual bool writeUint8( uint8 i)
	{
		return write((int8*)&i,1);
	}
	virtual bool writeInt8( int8 i)
	{
		return write((int8*)&i,1);
	}

	virtual bool writeUInt16( uint16 i) 
	{
		return write((int8*)&i,2);
	}
	virtual bool writeInt16( int16 i)
	{
		return write((int8*)&i,2);
	}

	virtual bool writeUInt32( uint32 i)
	{
		return	write((int8*)&i,4);
	}
	virtual bool writeInt32( int32 i)
	{
		return write((int8*)&i,4);
	}

	virtual bool writeInt64( int64 i)
	{
		return write((int8*)&i,8);
	}

	virtual bool writeFloat(float f)
	{
		return write((int8*)&f,4);
	}

	virtual bool writeString(const std::string& str)
	{
		if(write((int8*)str.length(),2))
		{
			if((size_t)(len_-curLen_)>=str.length())
			{
				memcpy(buf_+curLen_,str.c_str(),str.length());
				curLen_+=str.length();
				return true;
			}
		}
		return false;
	}
	virtual bool writeBin(const std::string& str)
	{
		return writeString(str);
	}
	bool write(int8* data,uint16 size)
	{
		if((len_-curLen_)>=size)
		{
			memcpy(buf_+curLen_,(int8*)&data,size);
			curLen_+=size;
			return true;
		}
		return false;
	}

	/** Reading functions */
	virtual bool readBool(bool& value)
	{
		return read((int8*)&value,1);
	}

	virtual bool readUint8(uint8& i)
	{
		return read((int8*)&i,1);
	}
	virtual bool readint8(int8& i)
	{
		return read((int8*)&i,1);
	}

	virtual bool readUInt16(uint16& i)
	{
		return read((int8*)&i,2);
	}
	virtual bool readInt16(int16& i)
	{
		return read((int8*)&i,2);
	}

	virtual bool readUInt32(uint32& i)
	{
		return read((int8*)&i,4);
	}
	virtual bool readInt32(int32& i)
	{
		return read((int8*)&i,4);
	}

	virtual bool readInt64(int64& i)
	{
		return read((int8*)&i,8);
	}
	virtual bool readFloat(float& f)
	{
		return read((int8*)&f,4);
	}

	virtual bool readString(std::string& str)
	{
		uint16 len=0;
		if(read((int8*)len,2))
		{
			str=std::string((const char*)buf_+curLen_,len);
			return true;
		}
		return false;
	}
	virtual bool readBin(std::string& str)
	{
		return readString(str);
	}
	bool read(int8* data,uint16 len)
	{
		if((len_-curLen_)>=len)
		{
			memcpy(data,buf_+curLen_,len);
			curLen_+=len;
			return true;
		}
		return false;
	}
	uint32 getCurLen()
	{
		return curLen_;
	}

private:
	uint32 len_;		//<buf最大长度
	uint32 curLen_;		//<当前长度
	int8*  buf_;		//<数据
};

#endif