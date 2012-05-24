//==============================================
/**
	@date:		2012:3:31  
	@file: 		IProtocol.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__IPROTOCOL_H__
#define		__IPROTOCOL_H__

#include "Common.h"
#include <string>
class IProtocol {
public:
	virtual ~IProtocol() {}
	virtual bool writeMsgBegin()=0;
	virtual bool writeMsgEnd()=0;
	virtual bool read(int8* data,uint32 len)=0;
	virtual bool write(int8* data,uint32 size)=0;

	virtual bool writeBool(bool value)
	{
		uint8 i=value?1:0;
		return write((int8*)&i,1);
	}

	virtual bool writeUInt8( uint8 i)
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
		uint16 len=str.length();
		if(write((int8*)&len,2))
		{
			return write((int8*)str.c_str(),str.length());
		}
		return false;
	}
	virtual bool writeBin(const std::string& str)
	{
		return writeString(str);
	}

	/** Reading functions */
	virtual bool readBool(bool& value)
	{
		return read((int8*)&value,1);
	}

	virtual bool readUInt8(uint8& i)
	{
		return read((int8*)&i,1);
	}
	virtual bool readInt8(int8& i)
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

	virtual bool readString(std::string& str)=0;

	virtual bool readBin(std::string& str)
	{
		return readString(str);
	}
};

#endif 
