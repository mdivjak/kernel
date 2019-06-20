#ifndef _shlist_h_
#define _shlist_h_
#include "../h/declare.h"

class SHList {
private:
	struct Elem {
		SignalHandler sh;
		Elem *next;
		Elem(SignalHandler s, Elem *n = 0);
	};

	Elem *first, *last;

	int contains(SignalHandler sh);
public:
	SHList();

	~SHList();

	void copy(SHList *sh);

	void deleteAll();
	void swap(SignalHandler sh1, SignalHandler sh2);
	void signalize();
	void add(SignalHandler s);
};

#endif
