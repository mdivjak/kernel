#ifndef _declare_h_
#define _declare_h_

#define NEW 0
#define READY 1
#define RUNNING 2
#define BLOCKED 3
#define TERMINATED 4

#define lock asm{\
	pushf;\
	cli;\
};
#define unlock asm popf;

typedef unsigned long StackSize;
typedef unsigned int Time;
typedef int ID;
typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

int syncPrintf(const char *format, ...);

#endif
