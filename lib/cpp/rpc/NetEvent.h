
#ifndef __RPC_NETEVENT_H__
#define __RPC_NETEVENT_H__

extern "C"{
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event2/buffer.h>
}
#include "EndPoint.h"

namespace rpc {
class NetEvent {

public:

	~NetEvent();

	static NetEvent* getInstance();

    typedef void (*event_callback_fn)(evutil_socket_t, short, void *);
    struct event_base* getEventBase(){ return base_; }


	event* scheduleTimer(event_callback_fn cb,void* arg, const timeval& delay, const timeval & interval);
	void cancelTimer(event* ev);

	int eventLoop();
	int endEventLoop();

private:
	static NetEvent*	event_;
	struct event_base*  base_;

private:
	NetEvent();
	NetEvent(const NetEvent&)=delete;
	NetEvent& operator=(const NetEvent&)=delete;
};
}
#endif 