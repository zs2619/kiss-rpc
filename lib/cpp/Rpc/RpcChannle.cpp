
#include "Transport.h"
#include "Protocol.h"
class RpcChannle{

public:
    RpcChannle(Transport* trans,Protocol* proto){
    }
    template<typename T>
    T createStub(){
        T t;
        return t;
    }

private:
    Transport trans_;
}