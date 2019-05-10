#ifndef _semaphor_h_
#define _semaphor_h_
#include "../h/declare.h"

class KernelSem;

class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual void signal(int n = 0);

	int val() const;
private:
	KernelSem *myImpl;
};

#endif
