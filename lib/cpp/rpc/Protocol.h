#include <vector>
#include "Common.h"
#include "Transport.h"

#ifndef __RPC_PROTOCOL_H__
#define __RPC_PROTOCOL_H__

class Protocol{
public:
    Protocol(){ 
    }
    virtual ~Protocol(){ 
    }

    virtual Protocol  createProtoBuffer(){
		return Protocol();
    }

    template<typename T>
    int write(T t){
		return 0;
    }

    template<typename T>
    int read(T t){
		return 0;
    }
    std::vector<int8> getBuffer(){
        return buff_;
    }
    void setBuffer(std::vector<int8> buff){
        buff_=buff;
    }

private:
    std::vector<int8> buff_;
};
class JsonProtocol :public Protocol{
};

class BinaryProtocol :public Protocol{
};
#endif
