#ifndef _thread_h_
#define _thread_h_
#include "declare.h"
#include "PCB.h"
class PCB;

const StackSize defaultStackSize = 4096;
const Time defaultTimeSlice = 2;

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
};

#endif // !_thread_h_
