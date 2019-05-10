#include <stdio.h>
#include <dos.h>
#include <iostream.h>
#include "../h/schedule.h"
#include "../h/PCB.h"
#include "../h/PCBList.h"
#include "../h/Thread.h"
#include "../h/idleThr.h"
#include "../h/declare.h"

#include "../h/KerSem.h"


volatile int cpuTime = 0;
volatile int changeContext = 0;

void dispatch();
void init();
void restore();
int userMain(int argc, char** argv) { return 0; }

//Da li running treba da bude volatile?
//Da li moze da se ceka nit koja nije pokrenuta ili se takvo cekanje ignorise?
//Da li treba da se uveca val kad istekne vreme cekanja niti na semaforu?
int main(int argc, char* argv[]) {
	//printf("Hello world\n");
	cpuTime = 5;
	PCB::running = new PCB(0, 5, 0);
	PCB::running->status = RUNNING;
	Thread *idleThread = new IdleThread();
	PCB::idlePCB = PCB::allPCBs.getPCBById(idleThread->getId());
	init();

	KernelSem ks(0);
	for(int i = 0; i < 5; i++) {
		syncPrintf("%d\n", i);
		ks.wait(18*55);
	}
	int retval = userMain(argc, argv);

	delete PCB::running;
	delete idleThread;
	restore();
	printf("retval = %d\n", retval);
	return retval;
}
