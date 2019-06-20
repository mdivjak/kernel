#ifndef _pcb_h_
#define _pcb_h_
#include "../h/PCBList.h"
#include "SHList.h"
#include "../h/declare.h"

class Thread;

class PCB {
private:
	static int sID;
public:
	friend class PCBList;
	PCB(StackSize size, Time slice, Thread *t);
	~PCB();

	unsigned sp;
	unsigned ss;
	unsigned bp;
	Time timeSlice;
	unsigned *stack;
	int status;
	ID id;
	PCBList waitingForMe;
	Thread *myThread;
	PCB* parent;

	static PCB *running;
	static PCB *idlePCB;
	static PCBList allPCBs;

	static void wrapper();

	void start();
	void waitToComplete();
	ID getId();
	static ID getRunningId();

private:
	//SIGNALI
	static int globalBlocks[16];
	int signalBlocks[16];
	SHList handlers[16];

	static int allowedSignals12;

	struct Elem {
		SignalId sig;
		Elem *next;
		Elem(SignalId s, Elem *n = 0);
	};

	Elem *first, *last;
	void remove(Elem *e);
public:
	int handleSignals();
	static void killSignal();

	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);
};

#endif
