#include <dos.h>
#include "../h/IVTEntry.h"
#include "../h/KernelEv.h"
#include "../h/declare.h"

IVTEntry* IVTEntry::allEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtno, pInterrupt inter) {
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtno);
	setvect(ivtno, inter);
#endif
	IVTEntry::allEntries[ivtno] = this;
	myNum = ivtno;
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	setvect(myNum, oldRoutine);
#endif
	IVTEntry::allEntries[myNum] = 0;
}

void IVTEntry::signal() {
	if(KernelEv::allEvents[myNum] != 0) {
		KernelEv::allEvents[myNum]->signal();
	}
}

void IVTEntry::callOld() {
	if(oldRoutine != 0)
		(*oldRoutine)();
}
