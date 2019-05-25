#include "../h/KerSem.h"
#include "../h/KSList.h"
#include "../h/BlkQueue.h"
#include "../h/PCB.h"
#include "../h/schedule.h"
#include "../h/declare.h"
#include "../h/thread.h"

KSList KernelSem::allKernelSems;

KernelSem::KernelSem(int init) {
	v = init;
	allKernelSems.add(this);
}

KernelSem::~KernelSem() {

lock();
	BlockedQueue::Elem *e = blockedQueue.get();
	while(e) {
		syncPrintf("Unblocking %d\n", e->pcb->id);
		e->pcb->status = READY;
		Scheduler::put(e->pcb);
		e = blockedQueue.get();
	}
unlock();

	allKernelSems.remove(this);
}

int KernelSem::val() const { return v; }

int KernelSem::wait(Time maxTimeToWait) {
	int retval = 1;
	if(--v < 0) {
lock();
		BlockedQueue::Elem *e = new BlockedQueue::Elem(PCB::running, maxTimeToWait, (maxTimeToWait ? 1 : 0));
		blockedQueue.add(e);
		PCB::running->status = BLOCKED;
unlock();
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

lock();
			BlockedQueue::Elem *e = blockedQueue.get();
			e->pcb->status = READY;
			Scheduler::put(e->pcb);
unlock();

			retval = 0;
		}
	} else if(n > 0) {

lock();
		int i = 0;
		while(i < n && blockedQueue.size() > 0) {
			BlockedQueue::Elem *e = blockedQueue.get();
			e->pcb->status = READY;
			Scheduler::put(e->pcb);
			i++;
		}
		v += n;
unlock();

		retval = i;
	} else retval = n;
	return retval;
}

void KernelSem::allKernelSemsTick() {
	for(KSList::Elem *cur = allKernelSems.first; cur; cur = cur->next) {
		for(BlockedQueue::Elem *e = cur->ks->blockedQueue.first; e; e = e->next) {
			if(e->time > 0) e->time--;
			if(e->time == 0 && e->wait == 1) {

lock();
				cur->ks->blockedQueue.remove(e);
				cur->ks->v++;
				e->pcb->status = READY;
				Scheduler::put(e->pcb);
unlock();

			}
		}
	}
}
