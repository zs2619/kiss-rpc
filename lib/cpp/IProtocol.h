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

	virtual bool writeBool(bool value)=0;

	virtual bool writeUint8( uint8 i)=0;
	virtual bool writeInt8( int8 i)=0;

	virtual bool writeUInt16( uint16 i) =0;
	virtual bool writeInt16( int16 i) =0;

	virtual bool writeUInt32( uint32 i) =0;
	virtual bool writeInt32( int32 i)=0;

	virtual bool writeInt64( int64 i)=0;

	virtual bool writeFloat(float f)=0;

	virtual bool writeString(const std::string& str)=0;

	virtual bool writeBin(const char* buf)=0;
	/** Reading functions */

	virtual bool readBool(bool& value)=0;

	virtual bool readUint8(uint8& i)=0;
	virtual bool readint8(int8& i)=0;

	virtual bool readUInt16(uint16& i)=0;
	virtual bool readInt16(int16& i)=0;

	virtual bool readUInt32(uint32& i)=0;
	virtual bool readInt32(int32& i)=0;

	virtual bool readInt64(int64& i)=0;

	virtual bool readFloat(float& f)=0;

	virtual bool readString(std::string& str)=0;

	virtual bool readBin(char buf)=0;
};

#endif 
