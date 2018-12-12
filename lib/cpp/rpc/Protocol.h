#include <vector>
#include "Common.h"
#include "Transport.h"
class Protocol{
public:
    virtual Protocol  createProtoBuffer(){
    }
    std:vector<int8> getBuffer(){
    }
    void setBuffer(std:vector<int8> buf){
    }

private:
    std:vector<int8> buf;
};