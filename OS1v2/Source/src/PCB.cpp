#include <stdio.h>
#include <dos.h>
#include "../h/PCB.h"
#include "../h/PCBList.h"
#include "../h/schedule.h"
#include "../h/declare.h"
#include "../h/thread.h"

int PCB::sID = 0;
PCB* PCB::running = 0;
PCB* PCB::idlePCB = 0;
PCBList PCB::allPCBs;

ID PCB::getId() {
	return id;
}

ID PCB::getRunningId() {
	return PCB::running->id;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	PCB::running->status = TERMINATED;
	PCB::running->waitingForMe.informThem();
	dispatch();
}

PCB::PCB(StackSize size, Time slice, Thread *t) {
	if (size > 65536)
		size = 65536;
	size /= sizeof(unsigned);
	id = ++sID;
	myThread = t;
	timeSlice = slice;
	status = NEW;
	stack = 0;

	if (size > 0) {
		lock
		stack = new unsigned[size];
		unlock
		stack[size - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
		stack[size - 2] = FP_SEG(PCB::wrapper);
		stack[size - 3] = FP_OFF(PCB::wrapper);
#endif
	}
	sp = bp = ss = 0;
#ifndef BCC_BLOCK_IGNORE
	bp = FP_OFF(stack + size - 12);
	sp = bp;
	ss = FP_SEG(stack + size - 12);
#endif
	PCB::allPCBs.add(this);
}

PCB::~PCB() {
	waitToComplete();
	PCB::allPCBs.remove(this->getId());
	if(stack)
		delete stack;
	stack = 0;
}

void PCB::start() {
	lock
	if(this != PCB::idlePCB && this->status == NEW) {
		syncPrintf("Starting %d\n", id);
		this->status = READY;
		Scheduler::put(this);
	}
	unlock
}

void PCB::waitToComplete() {
	if(this != PCB::running && this != PCB::idlePCB && this->status != TERMINATED && this->status != NEW) {
		syncPrintf("%d waiting %d\n", PCB::running->id, id);
		lock
		PCB::running->status = BLOCKED;
		//amWaitingFor++
		this->waitingForMe.add(PCB::running);
		unlock
		dispatch();
	}
}
