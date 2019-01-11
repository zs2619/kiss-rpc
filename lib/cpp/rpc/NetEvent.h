
#ifndef __RPC_NETEVENT_H__
#define __RPC_NETEVENT_H__

extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
}
#include <functional>  
#include <chrono>
#include "EndPoint.h"

namespace rpc {
class NetEvent {

public:

	~NetEvent();

	static NetEvent* getInstance();

    typedef void (*event_callback_fn)(evutil_socket_t, short, void *);
    struct event_base* getEventBase(){ return base_; }


	  template<class Rep1, class Period1, class Rep2 = int, class Period2 = std::ratio<1>>
	  int schedule_timer (std::function<void(evutil_socket_t, short, void *)> cb,
	                       const std::chrono::duration<Rep1, Period1>& delay,
	                       const std::chrono::duration<Rep2, Period2>& interval = std::chrono::duration<Rep2, Period2>::zero()){
	  }



	int cancelTimer(int id);

	int eventLoop();
	int endEventLoop();

private:
	static NetEvent*	event_;
	struct event_base*  base_;

private:
	NetEvent();
};
}
#endif 