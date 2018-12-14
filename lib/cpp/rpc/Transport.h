
#include "EndPoint.h"

class Transport{
public:
     EndPoint endpoint;
};
class TcpTransport :public Transport{

};
class HttpTransport :public Transport{
};