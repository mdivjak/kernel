#include "../h/PCBList.h"
#include "../h/PCB.h"
#include "../h/declare.h"
class PCB;

volatile int cpuTime = 20;
volatile int changeContext = 0;

PCB* running = 0;
PCB* idlePCB = 0;
PCB* mainPCB = 0;
Thread* uMain;
Thread* idleThread = 0;

//u visualu je unsigned 4B, na VM je valjda 2B
unsigned short tsp = 0;
unsigned short tss = 0;
unsigned short tbp = 0;

PCBList pcbList;

pInterrupt oldTimerRoutine = 0;