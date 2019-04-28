#include "../h/thread.h"
#include "../h/PCBList.h"
#include "../h/schedule.h"

extern PCBList pcbList;
extern PCB* running;
extern PCB* idlePCB;
void dispatch();

void Thread::start() {
	if (this->myPCB != idlePCB && this->myPCB->status != ThreadStatus::TERMINATED) {
		myPCB->status = ThreadStatus::READY;
		Scheduler::put(myPCB);
	}
}

void Thread::waitToComplete() {
	//ako running nije this i nit nije TERMINATED blokiraj je
	if (running != this->myPCB && this->myPCB->status != ThreadStatus::TERMINATED) {
		running->status = ThreadStatus::BLOCKED;
		dispatch();
	}
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	if (running)
		return running->id;
	return 0;
}

Thread * Thread::getThreadById(ID id) {
	Thread *t = pcbList.getThreadById(id);
	return t;
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}
