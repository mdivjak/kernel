#include "../h/PCBList.h"
#include "../h/PCB.h"
#include "../h/schedule.h"
#include "../h/declare.h"

PCBList::Elem::Elem(PCB *pp, Elem *pr = 0, Elem *nn = 0) {
	p = pp;
	next = nn;
	prev = pr;
}

PCBList::PCBList() {
	first = last = 0;
	n = 0;
}

int PCBList::size() {
	return n;
}

void PCBList::add(PCB *pp) {
	//mora atomicno jer se koristi new
	lock
	last = (!first ? first : last->next) = new Elem(pp, last);
	n++;
	unlock
}

void PCBList::remove(ID id) {
	Elem *cur = first;
	while(cur && cur->p->getId() != id) {
		cur = cur->next;
	}
	if(!cur) return;
	if(cur->prev) cur->prev->next = cur->next;
	if(cur->next) cur->next->prev = cur->prev;
	if(cur == first) first = first->next;
	if(cur == last) last = last->prev;
	n--;
	delete cur;
}

void PCBList::informThem() {
	for(Elem *cur = first; cur; cur = cur->next) {
		cur->p->amWaitingFor--;
		if(cur->p->amWaitingFor == 0) {
			lock
			cur->p->status = READY;
			Scheduler::put(cur->p);
			unlock
		}
	}
}

PCB* PCBList::getPCBById(ID id) {
	for(Elem *cur = first; cur; cur = cur->next) {
		if(cur->p && cur->p->getId() == id)
			return cur->p;
	}
	return 0;
}

PCBList::~PCBList() {
	while(first) {
		last = first;
		first = first->next;
		delete last;
	}
	first = last = 0;
	n = 0;
}
