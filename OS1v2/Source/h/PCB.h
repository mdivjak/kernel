#ifndef _pcb_h_
#define _pcb_h_
#include "declare.h"
#include "thread.h"
class Thread;

class PCB {
private:
	static ID ThreadID;
public:
	PCB(StackSize size, Time slice, Thread* thread);

	~PCB();

	unsigned sp;
	unsigned ss;
	unsigned bp;
	Time timeSlice;
	unsigned *stack;
	ThreadStatus status;
	ID id;
	Thread* myThread;

	static void wrapper();
};
#endif // !_pcb_h_
