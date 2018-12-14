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

private:
};