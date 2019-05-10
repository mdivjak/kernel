#ifndef _ks_list_h_
#define _ks_list_h_
#include "../h/declare.h"

class KernelSem;

class KSList {
private:
	struct Elem {
		KernelSem *ks;
		Elem *next, *prev;
		Elem(KernelSem *kersem, Elem *pr, Elem *nn);
	};

	Elem *first, *last;
	int n;
public:
	friend class KernelSem;
	KSList();
	~KSList();

	int size() const;
	void add(KernelSem *ks);
	void remove(KernelSem *ks);
};

#endif
