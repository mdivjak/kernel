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
lock();
	PCB::running->status = TERMINATED;
	PCB::running->waitingForMe.informThem();

	PCB::allowedSignals12 = 1;
	if(PCB::running->parent != 0) {
		//syncPrintf("%d ima roditelja\n", PCB::running->id);
		PCB::running->parent->signal(1);
	}
	PCB::running->signal(2);
	PCB::allowedSignals12 = 0;

	PCB::running->handleSignals();
unlock();
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
	//SIGNALI
	parent = PCB::running;
	for(int i = 0; i < 16; i++)
		signalBlocks[i] = 0;
	first = last = 0;
	handlers[0].add(PCB::killSignal);
	if(parent != 0) {
		for(int i = 0; i < 16; i++) {
			signalBlocks[i] = parent->signalBlocks[i];
			if(i != 0)
				handlers[i].copy(&(parent->handlers[i]));
		}
	}

	if (size > 0) {

lock();
		stack = new unsigned[size];
unlock();

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
lock();
	if(this != PCB::idlePCB && this->status == NEW) {
		syncPrintf("Starting %d\n", id);
		this->status = READY;
		Scheduler::put(this);
	}
unlock();
}

void PCB::waitToComplete() {
	if(this != PCB::running && this != PCB::idlePCB && this->status != TERMINATED && this->status != NEW) {

lock();
		syncPrintf("%d waiting %d\n", PCB::running->id, id);
		PCB::running->status = BLOCKED;
		this->waitingForMe.add(PCB::running);
unlock();
dispatch();
	}
}

//SIGNALI
int PCB::globalBlocks[16] = {0};
int PCB::allowedSignals12 = 0;

void PCB::blockSignal(SignalId signal) {
	if(signal < 0 || signal > 15) return;
	signalBlocks[signal] = 1;
}

void PCB::blockSignalGlobally(SignalId signal) {
	if(signal < 0 || signal > 15) return;
	PCB::globalBlocks[signal] = 1;
}

void PCB::unblockSignal(SignalId signal) {
	if(signal < 0 || signal > 15) return;
	signalBlocks[signal] = 0;
}

void PCB::unblockSignalGlobally(SignalId signal) {
	if(signal < 0 || signal > 15) return;
	PCB::globalBlocks[signal] = 0;
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	//ne sme da se registruje handler za signal 0
	if((signal < 1) || (signal > 15)) return;
	syncPrintf("Nit %d: Registrujem handler za signal %d\n", this->id, signal);
	handlers[signal].add(handler);
}

void PCB::unregisterAllHandlers(SignalId signal) {
	//ne sme da se brise handler za signal 0
	if(signal < 1 || signal > 15) return;
	handlers[signal].deleteAll();
}

void PCB::swap(SignalId signal, SignalHandler hand1, SignalHandler hand2) {
	if(signal < 0 || signal > 15) return;
	handlers[signal].swap(hand1, hand2);
}

PCB::Elem::Elem(SignalId s, Elem *n) {
	sig = s;
	next = n;
}

void PCB::remove(Elem *e) {
	Elem *cur = first, *prev = 0;
	while(cur && cur != e) {
		prev = cur;
		cur = cur->next;
	}
	if(!cur) return;

lock();
	if(prev) prev->next = cur->next;
	else first = cur->next;
	if(!cur->next) {
		last = prev;
		if(last)
			last->next = 0;
	}
	delete e;
unlock();
}

int PCB::handleSignals() {
	Elem *cur = first, *old = 0;
	int killed = 0;
	while(cur) {
		SignalId sig = cur->sig;
		old = 0;
		if(!globalBlocks[sig] && !signalBlocks[sig]) {
			syncPrintf("Nit %d: obradjujem signal %d\n", PCB::running->id, sig);

lock();
			handlers[sig].signalize();
unlock();

			if(sig == 0)
				killed = 1;
			old = cur;
		}
		cur = cur->next;
		if(old != 0) {
			//syncPrintf("Nit %d: brisem pristigli signal %d\n", PCB::running->id, sig);
			remove(old);
		}
		if(sig == 0)
			return 1;
	}
	return killed;
}

void PCB::killSignal() {
	syncPrintf("ubijam %d\n", PCB::running->id);

lock();
	PCB::running->status = TERMINATED;
	PCB::running->waitingForMe.informThem();

	PCB::allowedSignals12 = 1;
	if(PCB::running->parent != 0)
		PCB::running->parent->signal(1);
	PCB::running->signal(2);
	PCB::allowedSignals12 = 0;
unlock();

}

void PCB::signal(SignalId signal) {
	if(signal < 0 || signal > 15) return;
	if(signal == 1 && PCB::allowedSignals12 == 0) {
		//syncPrintf("Ne primam signal 1 od %d\n", PCB::running->id);
		return;
	}
	if(signal == 2 && PCB::allowedSignals12 == 0) {
		//syncPrintf("Ne primam signal 2 od %d\n", PCB::running->id);
		return;
	}
	//syncPrintf("Nit %d: ubacujem signal %d\n", PCB::running->id, signal);
lock();
	last = (!first ? first : last->next) = new Elem(signal);
unlock();
}
