#ifndef _kernelev_h_
#define _kernelev_h_
#include "../h/declare.h"

class PCB;

class KernelEv {
private:
	PCB* owner;
	PCB* blocked;
	IVTNo myNum;
	int val;
public:
	KernelEv(IVTNo ivtno);
	~KernelEv();

	static KernelEv* allEvents[256];
	void wait();
protected:
	friend class IVTEntry;
	friend class Event;
	void signal();
};

#endif
