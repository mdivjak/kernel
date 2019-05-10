#include <stdio.h>
#include "../h/PCB.h"
#include "../h/KerSem.h"
#include "../h/schedule.h"
#include "../h/declare.h"

extern volatile int cpuTime;
extern volatile int changeContext;

static unsigned tsp;
static unsigned tss;
static unsigned tbp;

void tick() {}

void interrupt timer(...) {
	if (!changeContext && cpuTime > 0) cpuTime--;

	KernelSem::allKernelSemsTick();

	if ((cpuTime == 0 && PCB::running->timeSlice != 0) || changeContext) {

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
#endif

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (PCB::running && PCB::running != PCB::idlePCB && PCB::running->status == RUNNING) {
			PCB::running->status = READY;
			Scheduler::put(PCB::running);
		}

		PCB::running = Scheduler::get();

		if (!PCB::running)
			PCB::running = PCB::idlePCB;
		PCB::running->status = RUNNING;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
		cpuTime = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
#endif
	}

	if(!changeContext) {
#ifndef BCC_BLOCK_IGNORE
		tick();
		asm int 60h;
#endif
	}
	changeContext = 0;
}
