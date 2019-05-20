#include "../h/KernelEv.h"
#include "../h/PCB.h"
#include "../h/schedule.h"
#include "../h/declare.h"
#include "../h/thread.h"

KernelEv* KernelEv::allEvents[256] = {0};

KernelEv::KernelEv(IVTNo ivtno) {
	myNum = ivtno;
	val = 0;
	blocked = 0;
	owner = PCB::running;
	KernelEv::allEvents[myNum] = this;
}

KernelEv::~KernelEv() {
	if(blocked != 0) {
		lock
		blocked->status = READY;
		Scheduler::put(blocked);
		unlock
		owner = 0;
		blocked = 0;
	}
	KernelEv::allEvents[myNum] = 0;
}

void KernelEv::wait() {
	if(PCB::running != owner) return;
	if(val == 0) {
		lock
		blocked = owner;
		blocked->status = BLOCKED;
		dispatch();
		unlock
	} else if(val == 1)
		val = 0;
}

void KernelEv::signal() {
	if(blocked == 0) val = 1;
	else {
		lock
		blocked->status = READY;
		Scheduler::put(blocked);
		blocked = 0;
		unlock
	}
}
