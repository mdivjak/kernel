#ifndef _blocked_queue_h_
#define _blocked_queue_h_
#include "../h/declare.h"

class PCB;
class KernelSem;

class BlockedQueue {
private:
	struct Elem {
		PCB *pcb;
		Time time;
		KernelSem *ks;
		int wait;
		Elem *next;
		Elem(PCB *pp, Time t, KernelSem *ks, int w);
	};

	Elem *first, *last;
	int n;
public:
	friend class KernelSem;
	BlockedQueue();
	~BlockedQueue();

	void add(Elem *e);
	Elem* get();
	void remove(Elem *e);
	int size() const;
};

#endif
