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

		virtual bool writeBool(bool value) = 0;
		virtual bool writeUInt8(uint8 i) = 0;
		virtual bool writeInt8(int8 i) = 0;
		virtual bool writeUInt16(uint16 i) = 0;
		virtual bool writeInt16(int16 i) = 0;
		virtual bool writeUInt32(uint32 i) = 0;
		virtual bool writeInt32(int32 i) = 0;
		virtual bool writeInt64(int64 i) = 0;
		virtual bool writeFloat(float f) = 0;
		virtual bool writeString(const std::string& str) = 0;

		virtual bool readBool(bool& value) = 0;
		virtual bool readUInt8(uint8& i) = 0;
		virtual bool readInt8(int8& i) = 0;
		virtual bool readUInt16(uint16& i) = 0;
		virtual bool readInt16(int16& i) = 0;
		virtual bool readUInt32(uint32& i) = 0;
		virtual bool readInt32(int32& i) = 0;
		virtual bool readInt64(int64& i) = 0;
		virtual bool readFloat(float& f) = 0;
		virtual bool readString(std::string& str) = 0;

		template<typename T>
		bool writeVec(std::vector<T>& vIn) {
			if (vIn.empty()) {
				return false;
			}
			writeUInt16((uint16)vIn.size());
			for (size_t i = 0; i < vIn.size(); i++)
			{
				if (std::is_same<int8, T>::value) {
					writeInt8(vIn[i]);
				}
				else if (std::is_same<uint8, T>::value) {
					writeUInt8(vIn[i]);
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

		std::vector<int8> getBuffer()const {
			return buff_;
		}
		void setBuffer(std::vector<int8> buff) {
			buff_ = buff;
		}

	protected:
		std::vector<int8> buff_;
        size_t curRead_;
        size_t curWrite_;
	};
	class JsonProtocol :public IProtocol {
	public:

		virtual bool writeBool(bool value) { return true; }
		virtual bool writeUInt8(uint8 i) { return true; }
		virtual bool writeInt8(int8 i) { return true; }
		virtual bool writeUInt16(uint16 i) { return true; }
		virtual bool writeInt16(int16 i) { return true; }
		virtual bool writeUInt32(uint32 i) { return true; }
		virtual bool writeInt32(int32 i) { return true; }
		virtual bool writeInt64(int64 i) { return true; }
		virtual bool writeFloat(float f) { return true; }
		virtual bool writeString(const std::string& str) { return true; };

		virtual bool readBool(bool& value) { return true; };
		virtual bool readUInt8(uint8& i) { return true; };
		virtual bool readInt8(int8& i) { return true; };
		virtual bool readUInt16(uint16& i) { return true; };
		virtual bool readInt16(int16& i) { return true; };
		virtual bool readUInt32(uint32& i) { return true; };
		virtual bool readInt32(int32& i) { return true; };
		virtual bool readInt64(int64& i) { return true; };
		virtual bool readFloat(float& f) { return true; };
		virtual bool readString(std::string& str) { return true; };

		virtual IProtocol*  createProtoBuffer() {
			return new JsonProtocol();
		}
	};

	class BinaryProtocol :public IProtocol {
	public:

		bool write(int8* data, uint32 len)
		{
			buff_.insert(buff_.end(), data, data + len);
            curWrite_+=len;
            return true;
		}

		virtual bool writeBool(bool value) { 
            uint8 i=value?1:0;
            return write((int8*)&i,1);
        }

		virtual bool writeUInt8(uint8 i) {
    		return write((int8*)&i,1);
        }
		virtual bool writeInt8(int8 i) {
    		return write((int8*)&i,1);
        }
		virtual bool writeUInt16(uint16 i) { 
		    return write((int8*)&i,2);
        }
		virtual bool writeInt16(int16 i) {
		    return write((int8*)&i,2);
        }
		virtual bool writeUInt32(uint32 i) {
            return	write((int8*)&i,4);
        }
		virtual bool writeInt32(int32 i) { 
            return	write((int8*)&i,4);
        }
		virtual bool writeInt64(int64 i) { 
		    return write((int8*)&i,8);
        }

		virtual bool writeFloat(float f) {
            return write((int8*)&f,4);
	    }

		virtual bool writeString(const std::string& str) {
            uint16 len=uint16(str.length());
            if(write((int8*)&len,2))
            {
                return write((int8*)str.c_str(),uint32(str.length()));
            }
            return false;
        };

        bool read(int8* data,uint32 len)
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
		virtual bool readBool(bool& value) { 
		    return read((int8*)&value,1);
        };
		virtual bool readUInt8(uint8& i) { 
		    return read((int8*)&i,1);
        };
		virtual bool readInt8(int8& i) {
		    return read((int8*)&i,1);
        };
		virtual bool readUInt16(uint16& i) {
		    return read((int8*)&i,2);
        };
		virtual bool readInt16(int16& i) {
		    return read((int8*)&i,2);
        };
		virtual bool readUInt32(uint32& i) {
		    return read((int8*)&i,4);
        };
		virtual bool readInt32(int32& i) {
		    return read((int8*)&i,4);
        };
		virtual bool readInt64(int64& i) { 
		    return read((int8*)&i,8);
        };
		virtual bool readFloat(float& f) {
		    return read((int8*)&f,4);
        }
		virtual bool readString(std::string& str) { 
            uint16	len=0;
            if (read((int8*)&len,2))
            {
                str=std::string((char*)buff_.data()+curRead_,len);
                curRead_+=len;
                return true;
            }
		    return false;
        }


		virtual IProtocol*  createProtoBuffer() {
			return new BinaryProtocol();
		}
	};
}
#endif
