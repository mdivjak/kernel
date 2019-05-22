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
	asm int 8h;
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

//SIGNALI

void Thread::blockSignal(SignalId signal) {
	myPCB->blockSignal(signal);
}

void Thread::blockSignalGlobally(SignalId signal) {
	PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal) {
	myPCB->unblockSignal(signal);
}

void Thread::unblockSignalGlobally(SignalId signal) {
	PCB::unblockSignalGlobally(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
	myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	myPCB->swap(id, hand1, hand2);
}

void Thread::signal(SignalId signal) {
	myPCB->signal(signal);
}
