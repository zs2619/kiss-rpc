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

	/**  Writing functions. */
	bool writeMessageBegin(const std::string& name) { }
	bool writeMessageEnd() { }

	bool writeStructBegin(const char* name) { }
	bool writeStructEnd() { }

	bool writeFieldBegin(const char* name) { }
	bool writeFieldEnd() { }

	bool writeMapBegin() { }
	bool writeMapEnd() { }

	bool writeArrayBegin() { } 
	bool writeArrayEnd() { }

	bool writeBool(bool value) { }

	bool writeUint8( uint8 i) { }
	bool writeInt8( int8 i) { } 

	bool writeUInt16( uint16 i) { }
	bool writeInt16( int16 i) { }

	bool writeUInt32( uint32 i) { }
	bool writeInt32( int32 i) { }

	bool writeInt64( int64 i) { }

	bool writeFloat(float f) { }

	bool writeString(const std::string& str) { }

	/** Reading functions */
	bool readMessageBegin(std::string& name) { }
	bool readMessageEnd() { }

	bool readStructBegin(std::string& name) { }
	bool readStructEnd() { }

	bool readFieldBegin(std::string& name) { }
	bool readFieldEnd() { }

	bool readMapBegin() { }
	bool readMapEnd() { }

	bool readArrayBegin() { }
	bool readArrayEnd() { }

	bool readBool(bool& value) { }

	bool readUint8(uint8 i) { }
	bool readint8(int8 i) { }

	bool readUInt16(uint16& i) { }
	bool readInt16(int16& i) { }

	bool readUInt32(uint32& i) { }
	bool readInt32(int32& i) { }

	bool readInt64(int64& i) { }

	bool readFloat(float& f) { }

	bool readString(std::string& str) { }
};

#endif 
