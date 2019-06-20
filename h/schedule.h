#ifndef _shedule_h_
#define _shedule_h_

class PCB;

class Scheduler {
public:
	static void put(PCB *);
	static PCB* get();
};

#endif // !_shedule_h_
