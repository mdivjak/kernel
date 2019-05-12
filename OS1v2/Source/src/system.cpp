#include <dos.h>

void interrupt timer(...);
typedef void interrupt (*pInterrupt)(...);

static pInterrupt oldRoutine;

void init() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldRoutine = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldRoutine);
	asm sti;
#endif
}

void restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x8, oldRoutine);
	asm sti;
#endif
}
