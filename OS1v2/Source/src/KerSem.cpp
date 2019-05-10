#include "../h/KerSem.h"
#include "../h/KSList.h"
#include "../h/BlkQueue.h"
#include "../h/PCB.h"
#include "../h/schedule.h"
#include "../h/Thread.h"
#include "../h/declare.h"

KSList KernelSem::allKernelSems;

KernelSem::KernelSem(int init) {
	v = init;
	allKernelSems.add(this);
}

KernelSem::~KernelSem() {
	allKernelSems.remove(this);
}

int KernelSem::val() const { return v; }

int KernelSem::wait(Time maxTimeToWait) {
	maxTimeToWait /= 55;
	int retval = 1;
	if(--v < 0) {
		lock
		BlockedQueue::Elem *e = new BlockedQueue::Elem(PCB::running, maxTimeToWait, this, (maxTimeToWait ? 1 : 0));
		blockedQueue.add(e);
		PCB::running->status = BLOCKED;
		unlock
		dispatch();
		retval = (e->time == 0 && maxTimeToWait) ? 0 : 1;
		delete e;
	}
	return retval;
}

int KernelSem::signal(int n) {
	int retval;
	if(n == 0) {
		//obican signal
		if(v++ < 0) {
			lock
			BlockedQueue::Elem *e = blockedQueue.get();
			e->pcb->status = READY;
			Scheduler::put(e->pcb);
			retval = 0;
			unlock
		}
	} else if(n > 0) {
		lock
		int i = 0;
		while(i < n && blockedQueue.size() > 0) {
			BlockedQueue::Elem *e = blockedQueue.get();
			e->pcb->status = READY;
			Scheduler::put(e->pcb);
			i++;
		}
		v += n;
		retval = i;
		unlock
	} else retval = n;
	return retval;
}

void KernelSem::allKernelSemsTick() {
	KSList::Elem *cur = allKernelSems.first;
	for(; cur; cur = cur->next) {
		KernelSem *ks = cur->ks;
		BlockedQueue::Elem *e = ks->blockedQueue.first;
		for(; e; e = e->next) {
			if(e->time > 0) e->time--;
			if(e->time == 0 && e->wait == 1) {
				lock
				ks->blockedQueue.remove(e);
				//da li treba da se poveca val?
				//ks->v++;
				e->pcb->status = READY;
				Scheduler::put(e->pcb);
				unlock
			}
		}
	}
}
