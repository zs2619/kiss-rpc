#include <sstream>
#include <assert.h>
#include "EndPoint.h"

 rpc::EndPoint::EndPoint(const std::string & url) {
	memset(&sockAddrIn_, 0, sizeof(sockaddr_in));
	if (-1 == set(url)) {
	}
}

 inline int rpc::EndPoint::set(const std::string & url) {
	 std::size_t pos = url.find(":");
	 std::string host = url.substr(0, pos);
	 std::string portStr = url.substr(pos + 1, url.length());

	 struct evutil_addrinfo *ai = nullptr;
	 struct evutil_addrinfo hints;
	 memset(&hints, 0, sizeof(hints));
	 hints.ai_family = PF_UNSPEC;
	 hints.ai_socktype = SOCK_STREAM;
	 int r = evutil_getaddrinfo(host.c_str(), portStr.c_str(), &hints, &ai);
	 if (r != 0) {
		 return -1;
	 }

	 struct sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(ai->ai_addr);
	 sockAddrIn_ = *sin;
	 hostname_ = host;
	 evutil_freeaddrinfo(ai);
	 return 0;
 }
