#ifndef __RPC_PROTOCOL_H__
#define __RPC_PROTOCOL_H__

#include <vector>
#include "rpc/Common.h"
namespace rpc {

class IProtocol {
public:

	IProtocol():curRead_(0),curWrite_(0) {
	}

	virtual ~IProtocol() {
	}

	virtual IProtocol*  createProtoBuffer() {
		return nullptr;
	};

	virtual bool writeBool(bool b) = 0;
	virtual bool writeByte(byte by) = 0;
	virtual bool writeInt8(int8 i) = 0;
	virtual bool writeInt16(int16 i) = 0;
	virtual bool writeInt32(int32 i) = 0;
	virtual bool writeInt64(int64 i) = 0;
	virtual bool writeFloat(float f) = 0;
	virtual bool writeString(const std::string& str) = 0;
	virtual bool writeBinary(const std::vector<byte>& buff) = 0;

	virtual bool readBool(bool& b) = 0;
	virtual bool readByte(byte& by) = 0;
	virtual bool readInt8(int8& i) = 0;
	virtual bool readInt16(int16& i) = 0;
	virtual bool readInt32(int32& i) = 0;
	virtual bool readInt64(int64& i) = 0;
	virtual bool readFloat(float& f) = 0;
	virtual bool readString(std::string& str) = 0;
	virtual bool readBinary(std::vector<byte>& buff) = 0;

	template<typename T>
	bool writeVec(std::vector<T>& vIn) {
		if (vIn.empty()) {
			return false;
		}
		writeUInt16((int32)vIn.size());
		for (size_t i = 0; i < vIn.size(); i++)
		{
			if (std::is_same<int8, T>::value) {
				writeInt8(vIn[i]);
			}
			else if (std::is_same<byte, T>::value) {
				writeByte(vIn[i]);
			}
		}
		return true;
	}

	template<typename T>
	bool read(std::vector<T>& vOut) {
		if (buff_.empty())
			return false;

		return 0;
	}

	std::vector<byte> getBuffer()const {
		return buff_;
	}
	void setBuffer(std::vector<byte> buff) {
		buff_ = buff;
	}

protected:
	std::vector<byte> buff_;
	size_t curRead_;
	size_t curWrite_;
};
class JsonProtocol :public IProtocol {
public:

	virtual bool writeBool(bool b) { return true; }
	virtual bool writeByte(byte by) { return true; }
	virtual bool writeInt8(int8 i) { return true; }
	virtual bool writeInt16(int16 i) { return true; }
	virtual bool writeInt32(int32 i) { return true; }
	virtual bool writeInt64(int64 i) { return true; }
	virtual bool writeFloat(float f) { return true; }
	virtual bool writeString(const std::string& str) { return true; };
	virtual bool writeBinary(const std::vector<byte>& buff) {return true;};

	virtual bool readBool(bool& b) { return true; };
	virtual bool readByte(uint8& by) { return true; };
	virtual bool readInt8(int8& i) { return true; };
	virtual bool readInt16(int16& i) { return true; };
	virtual bool readInt32(int32& i) { return true; };
	virtual bool readInt64(int64& i) { return true; };
	virtual bool readFloat(float& f) { return true; };
	virtual bool readString(std::string& str) { return true; };
	virtual bool readBinary(std::vector<byte>& buff) {return true;};

	virtual IProtocol*  createProtoBuffer() {
		return new JsonProtocol();
	}
};

class BinaryProtocol :public IProtocol {
public:

	bool write(byte* data, size_t len)
	{
		buff_.insert(buff_.end(), data, data + len);
		curWrite_+=len;
		return true;
	}

	virtual bool writeBool(bool b) { 
		uint8 i=b?1:0;
		return write((byte*)&i,1);
	}

	virtual bool writeByte(byte by) {
		return write((byte*)&by,1);
	}
	virtual bool writeInt8(int8 i) {
		return write((byte*)&i,1);
	}
	virtual bool writeInt16(int16 i) {
		return write((byte*)&i,2);
	}
	virtual bool writeInt32(int32 i) { 
		return	write((byte*)&i,4);
	}
	virtual bool writeInt64(int64 i) { 
		return write((byte*)&i,8);
	}

	virtual bool writeFloat(float f) {
		return write((byte*)&f,4);
	}

	virtual bool writeString(const std::string& str) {
		int32 len=int32(str.length());
		if(write((byte*)&len,4))
		{
			return write((byte*)str.c_str(),uint32(str.length()));
		}
		return false;
	};

	virtual bool writeBinary(const std::vector<byte>& buff) {
		int32 len=int32(buff.size());
		if(write((byte*)&len,4))
		{
			return write((byte*)buff_.data()+curRead_,len);
		}
		return false;
	};

	bool read(byte* data,size_t len)
	{
		if (len==0)
			return true;
		if(buff_.size()>=len)
		{
			memcpy(data,buff_.data()+curRead_,len);
			curRead_+=len;
			return true;
		}
		return false;
	}
	virtual bool readBool(bool& b) { 
		return read((byte*)&b,1);
	};
	virtual bool readByte(byte& by) { 
		return read((byte*)&by,1);
	};
	virtual bool readInt8(int8& i) {
		return read((byte*)&i,1);
	};
	virtual bool readInt16(int16& i) {
		return read((byte*)&i,2);
	};
	virtual bool readInt32(int32& i) {
		return read((byte*)&i,4);
	};
	virtual bool readInt64(int64& i) { 
		return read((byte*)&i,8);
	};
	virtual bool readFloat(float& f) {
		return read((byte*)&f,4);
	}
	virtual bool readString(std::string& str) { 
		int32	len=0;
		if (read((byte*)&len,4))
		{
			str=std::string((char*)buff_.data()+curRead_,len);
			curRead_+=len;
			return true;
		}
		return false;
	}
	virtual bool readBinary(std::vector<byte>& buff) {
		int32	len=0;
		if (read((byte*)&len,4))
		{
			buff.insert(buff.begin(), ((byte*)buff_.data()+curRead_,len));
			curRead_+=len;
			return true;
		}
		return false;

	};


	virtual IProtocol*  createProtoBuffer() {
		return new BinaryProtocol();
	}
};
}
#endif
