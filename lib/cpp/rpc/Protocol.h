#ifndef __RPC_PROTOCOL_H__
#define __RPC_PROTOCOL_H__

#include <vector>
#include "Common.h"

class IProtocol{
public:
    IProtocol(){ 
    }
    virtual ~IProtocol(){ 
    }

	virtual IProtocol*  createProtoBuffer() { 
		return nullptr;
	};

	virtual bool writeBool(bool value)=0;
	virtual bool writeUInt8( uint8 i)=0;
	virtual bool writeInt8( int8 i)=0;
	virtual bool writeUInt16( uint16 i)=0; 
	virtual bool writeInt16( int16 i)=0;
	virtual bool writeUInt32( uint32 i)=0;
	virtual bool writeInt32( int32 i)=0;
	virtual bool writeInt64( int64 i)=0;
	virtual bool writeFloat(float f)=0;
	virtual bool writeString(const std::string& str)=0;

	virtual bool readBool(bool& value)=0;
	virtual bool readUInt8(uint8& i)=0;
	virtual bool readInt8(int8& i)=0;
	virtual bool readUInt16(uint16& i)=0;
	virtual bool readInt16(int16& i)=0;
	virtual bool readUInt32(uint32& i)=0;
	virtual bool readInt32(int32& i)=0;
	virtual bool readInt64(int64& i)=0;
	virtual bool readFloat(float& f)=0;
	virtual bool readString(std::string& str)=0;

    template<typename T>
    bool writeVec(std::vector<T>& vIn){
        if (vIn.empty()){
            return false;
        }
		writeUInt16((uint16)vIn.size());
		for (size_t i=0;i<vIn.size();i++)
		{
            if (std::is_same<int8,T>::value){
				writeInt8(vIn[i]);
			} else if (std::is_same<uint8,T>::value){
				writeUInt8(vIn[i]);
			}
        }
		return true;
    }

    template<typename T>
    bool read(std::vector<T>& vOut){
        if (buff_.empty())
            return false;

		return 0;
    }

    std::vector<int8> getBuffer()const{
        return buff_;
    }
    void setBuffer(std::vector<int8> buff){
        buff_=buff;
    }

private:
    std::vector<int8> buff_;
};
class JsonProtocol :public IProtocol{
public:

	virtual bool writeBool(bool value) { return true; }
	virtual bool writeUInt8( uint8 i){return true;}
	virtual bool writeInt8( int8 i){return true;}
	virtual bool writeUInt16( uint16 i){return true;}
	virtual bool writeInt16( int16 i){return true;}
	virtual bool writeUInt32( uint32 i){return true;}
	virtual bool writeInt32( int32 i){return true;}
	virtual bool writeInt64( int64 i){return true;}
	virtual bool writeFloat(float f){return true;}
	virtual bool writeString(const std::string& str){return true;};

	virtual bool readBool(bool& value){return true;};
	virtual bool readUInt8(uint8& i){return true;};
	virtual bool readInt8(int8& i){return true;};
	virtual bool readUInt16(uint16& i){return true;};
	virtual bool readInt16(int16& i){return true;};
	virtual bool readUInt32(uint32& i){return true;};
	virtual bool readInt32(int32& i){return true;};
	virtual bool readInt64(int64& i){return true;};
	virtual bool readFloat(float& f){return true;};
	virtual bool readString(std::string& str){return true;};

    virtual IProtocol*  createProtoBuffer(){
		return new JsonProtocol();
    }
};

class BinaryProtocol :public IProtocol{
public:
	virtual bool writeBool(bool value) { return true; }
	virtual bool writeUInt8( uint8 i){return true;}
	virtual bool writeInt8( int8 i){return true;}
	virtual bool writeUInt16( uint16 i){return true;}
	virtual bool writeInt16( int16 i){return true;}
	virtual bool writeUInt32( uint32 i){return true;}
	virtual bool writeInt32( int32 i){return true;}
	virtual bool writeInt64( int64 i){return true;}
	virtual bool writeFloat(float f){return true;}
	virtual bool writeString(const std::string& str){return true;};

	virtual bool readBool(bool& value){return true;};
	virtual bool readUInt8(uint8& i){return true;};
	virtual bool readInt8(int8& i){return true;};
	virtual bool readUInt16(uint16& i){return true;};
	virtual bool readInt16(int16& i){return true;};
	virtual bool readUInt32(uint32& i){return true;};
	virtual bool readInt32(int32& i){return true;};
	virtual bool readInt64(int64& i){return true;};
	virtual bool readFloat(float& f){return true;};
	virtual bool readString(std::string& str){return true;};

    virtual IProtocol*  createProtoBuffer(){
		return new BinaryProtocol();
    }
};
#endif
