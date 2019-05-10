#ifndef _kernel_sem_h_
#define _kernel_sem_h_
#include "../h/KSList.h"
#include "../h/BlkQueue.h"
#include "../h/declare.h"

class KernelSem {
public:
	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	int val() const;

	static KSList allKernelSems;
	static void allKernelSemsTick();
private:
	int v;
	BlockedQueue blockedQueue;
};

#endif
