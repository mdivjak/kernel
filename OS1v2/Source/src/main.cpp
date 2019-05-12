#include <stdio.h>
#include <dos.h>
#include <iostream.h>
#include "../h/schedule.h"
#include "../h/PCB.h"
#include "../h/PCBList.h"
#include "../h/idleThr.h"
#include "../h/semaphor.h"
#include "../h/declare.h"
#include "../h/thread.h"


volatile int cpuTime = 0;
volatile int changeContext = 0;

void init();
void restore();
int userMain(int argc, char** argv);

int main(int argc, char* argv[]) {
	cpuTime = 5;
	PCB::running = new PCB(0, 5, 0);
	PCB::running->status = RUNNING;
	Thread *idleThread = new IdleThread();
	PCB::idlePCB = PCB::allPCBs.getPCBById(idleThread->getId());
	init();

	int retval = userMain(argc, argv);

	delete PCB::running;
	delete idleThread;
	restore();
	printf("retval = %d\n", retval);
	scanf("%d", &retval);
	return retval;
}
