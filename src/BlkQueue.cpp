#include "../h/BlkQueue.h"
#include "../h/PCB.h"
#include "../h/KerSem.h"
#include "../h/declare.h"

BlockedQueue::Elem::Elem(PCB *pp, Time t, int w) {
	pcb = pp;
	time = t;
	wait = w;
	next = 0;
}

BlockedQueue::BlockedQueue() {
	first = last = 0;
	n = 0;
}

void BlockedQueue::add(Elem *e) {
lock();
	last = (!first ? first : last->next) = e;
	n++;
unlock();
}

BlockedQueue::Elem* BlockedQueue::get() {
	if(n == 0) return 0;
	Elem *cur = first;

lock();
	first = first->next; n--;
unlock();

	cur->next = 0;
	return cur;
}

int BlockedQueue::size() const { return n; }

void BlockedQueue::remove(Elem *e) {
	Elem *cur = first, *prev = 0;
	while(cur && cur != e) {
		prev = cur;
		cur = cur->next;
	}
	if(!cur) return;

lock();
	if(prev) prev->next = cur->next;
	else first = cur->next;
	if(!cur->next) {
		last = prev;
		if(last) last->next = 0;
	}
	n--;
unlock();
}

BlockedQueue::~BlockedQueue() {
	while(first) {
		last = first;
		first = first->next;
		delete last;
	}
	first = last = 0;
	n = 0;
}
