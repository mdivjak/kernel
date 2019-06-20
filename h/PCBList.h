#ifndef _pcb_list_h_
#define _pcb_list_h_
#include "../h/declare.h"

class PCB;

class PCBList {
private:
	struct Elem {
		PCB *p;
		Elem *next, *prev;
		Elem(PCB *pp, Elem *pr, Elem *nn);
	};

	Elem *first, *last;
	int n;

	void deleteAll();
public:
	friend class PCB;
	PCBList();
	~PCBList();

	int size();
	void add(PCB *pp);
	void remove(ID id);
	void informThem();
	PCB* getPCBById(ID id);
};

#endif
