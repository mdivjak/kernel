#include <dos.h>
#include "../h/IVTEntry.h"
#include "../h/KernelEv.h"
#include "../h/declare.h"

IVTEntry* IVTEntry::allEntries[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtno, pInterrupt inter) {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldRoutine = getvect(ivtno);
	setvect(ivtno, inter);
	asm sti;
#endif
	IVTEntry::allEntries[ivtno] = this;
	myNum = ivtno;
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(myNum, oldRoutine);
	asm sti;
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
