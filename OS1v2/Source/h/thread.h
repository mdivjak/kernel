#ifndef _thread_h_
#define _thread_h_
#include "../h/declare.h"

class PCB;

extern const StackSize defaultStackSize;
extern const Time defaultTimeSlice;

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

protected:
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	PCB* myPCB;

public:
	//SIGNALI
	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
};

void dispatch();

#endif
