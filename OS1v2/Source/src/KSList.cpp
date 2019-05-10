#include "../h/KSList.h"
#include "../h/declare.h"

KSList::Elem::Elem(KernelSem *kersem, Elem *pr = 0, Elem *nn = 0) {
	ks = kersem;
	prev = pr;
	next = nn;
}

KSList::KSList() {
	first = last = 0;
	n = 0;
}

int KSList::size() const { return n; }

void KSList::add(KernelSem *kersem) {
	lock
	last = (!first ? first : last->next) = new Elem(kersem, last);
	n++;
	unlock
}

void KSList::remove(KernelSem *ks) {
	Elem *cur = first;
	while(cur && cur->ks != ks) cur = cur->next;

	if(!cur) return;
	if(cur->prev) cur->prev->next = cur->next;
	if(cur->next) cur->next->prev = cur->prev;
	if(cur == first) first = first->next;
	if(cur == last) last = last->prev;
	n--;
	delete cur;
}

KSList::~KSList() {
	while(first) {
		last = first;
		first = first->next;
		delete last;
	}
	first = last = 0;
	n = 0;
}
