#ifndef _idle_thread_h_
#define _idle_thread_h_

#include "../h/Thread.h"

class IdleThread : public Thread {
public:
	IdleThread();
protected:
	void run();
};

#endif