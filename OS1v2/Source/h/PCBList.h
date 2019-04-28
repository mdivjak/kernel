#ifndef _pcb_list_h_
#define _pcb_list_h_
#include "PCB.h"

class PCBList {
	struct Elem {
		PCB *p;
		Elem *next;
		Elem(PCB *p, Elem *next = 0) {
			this->p = p;
			this->next = next;
		}
	};

	Elem *first, *last;
	int n;
public:
	PCBList() {
		first = last = 0;
		n = 0;
	}

	~PCBList() {
		while (first) {
			last = first;
			first = first->next;
			delete last;
		}
		first = last = 0;
		n = 0;
	}

	PCBList& add(PCB *p) {
		last = (!first ? first : last->next) = new Elem(p);
		++n;
		return *this;
	}

	Thread* getThreadById(ID id) {
		for (Elem *cur = first; cur; cur = cur->next) {
			if (cur->p->myThread->getId() == id)
				return cur->p->myThread;
		}
		return 0;
	}

	PCB* getPCBById(ID id) {
		for (Elem *cur = first; cur; cur = cur->next) {
			if (cur->p->myThread && cur->p->myThread->getId() == id)
				return cur->p;
		}
		return 0;
	}
};
#endif // !_pcb_list_h_
