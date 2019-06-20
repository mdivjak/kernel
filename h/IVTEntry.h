#ifndef _ivtentry_h_
#define _ivtentry_h_
#include "../h/declare.h"
#include "../h/KernelEv.h"

class IVTEntry {
private:
	pInterrupt oldRoutine;
	IVTNo myNum;
public:
	IVTEntry(IVTNo ivtno, pInterrupt inter);
	~IVTEntry();

	static IVTEntry* allEntries[256];

	void signal();
	void callOld();
};

#endif
