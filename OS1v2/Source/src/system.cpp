#include <dos.h>

void interrupt timer(...);
typedef void interrupt (*pInterrupt)(...);

static pInterrupt oldRoutine;

void init() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldRoutine = getvect(0x1C);
	setvect(0x1C, timer);
	setvect(0x60, oldRoutine);
	asm sti;
#endif
}

void restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x1C, oldRoutine);
	asm sti;
#endif
}
