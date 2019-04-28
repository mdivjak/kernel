#include "../h/thread.h"
#include "../h/PCB.h"
#include "../h/idle.h"
#include "../h/userMain.h"
#include "../h/PCBList.h"
#include "../h/schedule.h"
#include <stdio.h>
#include <dos.h>

extern Thread* idleThread;
extern Thread* uMain;
extern volatile int cpuTime;
extern volatile int changeContext;
extern PCB* running;
extern PCB* idlePCB;
extern PCB* mainPCB;
extern unsigned short tsp;
extern unsigned short tss;
extern unsigned short tbp;
extern PCBList pcbList;
extern pInterrupt oldTimerRoutine;


//timer interrupt routine
void /*interrupt */timer() {
	if (!changeContext) cpuTime--;
	if ((cpuTime == 0 && running->timeSlice!=0) || changeContext) {
		//save necessary registers inside PCB
		__asm{
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		//save tsp tss tbp inside running PCB context
		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		//da li mora da se poredi running != mainPCB	
		if (running && running != idlePCB && running->status == ThreadStatus::RUNNING) {
			running->status = ThreadStatus::READY;
			Scheduler::put(running);
		}

		running = Scheduler::get();

		if (!running)
			running = pcbList.getPCBById(idleThread->getId());
		//ako je running nula postavi ga na idle
		//load sp ss bp from new context
		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;
		cpuTime = running->timeSlice;

		__asm{
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	} 
	//da li ovo treba u posebnom if(!changeContext)
	else {
		//call tick function and old interrupt routine
		//tick();
		__asm int 60h;
	}
	changeContext = 0;
}

void initialize() {
	/*lock
	oldTimerRoutine = getvect(1Ch);
	setvect(1Ch, timer);
	setvect(60h, oldTimerRoutine);
	unlock*/
}

void restore() {
	/*lock
	setvect(1Ch, oldTimerRoutine);
	unlock*/
}

void dispatch() {
	//asm cli;
	changeContext = 1;
	timer();
	//asm sti;
}


int main(int argc, char* argv[]) {
	mainPCB = new PCB(0, 1, 0);
	idleThread = new Idle();
	idlePCB = pcbList.getPCBById(idleThread->getId());
	uMain = new UserMain(argc, argv);
	//initialize();


	uMain->start();

	delete uMain;
	delete idleThread;

	//restore();
}