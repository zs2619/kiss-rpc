#include <vector>
#include "Common.h"
#include "Transport.h"

class Protocol{
public:
    Protocol(){ 
    }
    virtual ~Protocol(){ 
    }

    virtual Protocol  createProtoBuffer(){
    }

    template<typename T>
    int write(T t){
    }

    template<typename T>
    int read(T t){
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

