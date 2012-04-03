//==============================================
/**
	@date:		2012:3:31  
	@file: 		IProtocol.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__IPROTOCOL_H__
#define		__IPROTOCOL_H__

#include "RpcTypeDef.h"
class IProtocol {
public:
	virtual ~IProtocol() {}

	bool writeBool(bool value)=0;

	bool writeUint8( uint8 i)=0;
	bool writeInt8( int8 i)=0;

	bool writeUInt16( uint16 i) =0;
	bool writeInt16( int16 i) =0;

	bool writeUInt32( uint32 i) =0;
	bool writeInt32( int32 i)=0;

	bool writeInt64( int64 i)=0;

	bool writeFloat(float f)=0;

	bool writeString(const std::string& str)=0;

	/** Reading functions */

	bool readBool(bool& value)=0;

	bool readUint8(uint8 i)=0;
	bool readint8(int8 i)=0;

	bool readUInt16(uint16& i)=0;
	bool readInt16(int16& i)=0;

	bool readUInt32(uint32& i)=0;
	bool readInt32(int32& i)=0;

	bool readInt64(int64& i)=0;

	bool readFloat(float& f)=0;

	bool readString(std::string& str)=0;
};

#endif 
