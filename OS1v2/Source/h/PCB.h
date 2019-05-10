#ifndef _pcb_h_
#define _pcb_h_
#include "../h/PCBList.h"
#include "../h/declare.h"


class Thread;
//PCB CLASS
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

	static PCB *running;
	static PCB *idlePCB;
	static PCBList allPCBs;

	static void wrapper();

	void start();
	void waitToComplete();
	ID getId();
	static ID getRunningId();
};

#endif
