#include <dos.h>
#include <stdio.h>
#include "../h/declare.h"
#include "../h/PCB.h"
#include "../h/thread.h"
#include "../h/idleThr.h"

void interrupt timer(...);
static pInterrupt oldRoutine;

extern volatile int cpuTime;
Thread* idleThread = 0;

void lock() {
	//("Nit %d: zakljucavam %d\n", PCB::running->id, lockCounter);
	lockCounter++;
}

void unlock() {
	//printf("\tNit %d: otkljucavam %d\n", PCB::running->id, lockCounter);
	if(lockCounter == 0) return;
	lockCounter--;
}

void init() {
	cpuTime = 5;
	PCB::running = new PCB(0, 5, 0);
	PCB::running->status = RUNNING;
	idleThread = new IdleThread();
	PCB::idlePCB = PCB::allPCBs.getPCBById(idleThread->getId());

#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldRoutine = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldRoutine);
	asm sti;
#endif
}

void restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x8, oldRoutine);
	asm sti;
#endif
}
