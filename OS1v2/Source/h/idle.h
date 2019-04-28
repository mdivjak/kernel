#ifndef _idle_h_
#define _idle_h_
#include "thread.h"

class Idle : public Thread {
public:
	Idle() : Thread(512, 1) {}
protected:
	void run() override {
		while (1) {}
	}

	PCB* getPCB() {
		return 0;
	}
};

#endif // !_idle_h_
