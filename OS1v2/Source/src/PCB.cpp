#include <dos.h>
#include "../h/PCB.h"
#include "../h/PCBList.h"
#define PSW_INIT_VALUE 512

extern PCB* running;
extern PCBList pcbList;
void dispatch();

ID PCB::ThreadID = 0;

PCB::PCB(StackSize size, Time slice, Thread * thread) {
	if (size > 65536)
		size = 65536;

	id = ++ThreadID;
	myThread = thread;
	timeSlice = slice;
	status = ThreadStatus::NEW;
	stack = 0;

	//da li se alocira char ili unsigned?
	//for mainPCB we don't need to allocate stack
	if (size > 0) {
		stack = new unsigned[size / sizeof(unsigned)];
		stack[size - 1] = PSW_INIT_VALUE;
		stack[size - 2] = 0;// FP_SEG(PCB::wrapper);
		stack[size - 3] = 0;// FP_OFF(PCB::wrapper);
	}

	bp = sp = 0;// FP_OFF((char*)stack + size - 12);
	ss = 0;// FP_SEG((char*)stack + size - 12);

	pcbList.add(this);
}

PCB::~PCB() {
	if(stack)
		delete stack;
	stack = 0;
}

void PCB::wrapper() {
	running->myThread->run();
	running->status = TERMINATED;
	dispatch();
}
