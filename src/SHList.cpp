#include "../h/SHList.h"

SHList::Elem::Elem(SignalHandler s, Elem *n) {
	sh = s;
	next = n;
}

int SHList::contains(SignalHandler sh) {
	for(Elem *cur = first; cur; cur = cur->next)
		if(cur->sh == sh) return 1;
	return 0;
}

void SHList::copy(SHList *sh) {
	deleteAll();
	for(Elem *cur = sh->first; cur; cur = cur->next) {
		this->add(cur->sh);
	}
}

SHList::SHList() {
	first = 0;
	last = 0;
}

SHList::~SHList() {
	deleteAll();
}

void SHList::deleteAll() {
	while(first) {
		last = first;
		first = first->next;
		delete last;
	}
	first = last = 0;
}

void SHList::swap(SignalHandler sh1, SignalHandler sh2) {
	Elem *a = 0, *b = 0;
	for(Elem *cur = first; cur; cur = cur->next) {
		if(cur->sh == sh1) a = cur;
		if(cur->sh == sh2) b = cur;
		if(a != 0 && b != 0) break;
	}
	if(a == 0 || b == 0) return;
	SignalHandler tmp = a->sh;
	a->sh = b->sh;
	b->sh = tmp;
}

void SHList::signalize() {
	for(Elem *cur = first; cur; cur = cur->next) {
		(*(cur->sh))();
	}
}

void SHList::add(SignalHandler s) {
	//da li treba ova provera
	if(contains(s)) return;
lock();
	last = (!first ? first : last->next) = new Elem(s);
unlock();
}
