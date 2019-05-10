#include "../h/PCB.h"
#include "../h/PCBList.h"
#include "../h/thread.h"

const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2;

extern int changeContext;
void interrupt timer(...);

void dispatch() {
	lock
	changeContext = 1;
	timer();
	unlock
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
}

Thread::~Thread() {
	delete myPCB;
}

void Thread::start() {
	myPCB->start();
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getId() {
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::running->getId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::allPCBs.getPCBById(id)->myThread;
}
