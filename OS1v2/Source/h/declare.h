#ifndef _declare_h_
#define _declare_h_

typedef unsigned int Time;
typedef unsigned long StackSize;
typedef int ID;
typedef void /*interrupt*/ (*pInterrupt) (...);

enum ThreadStatus { NEW, READY, RUNNING, BLOCKED, TERMINATED };

#endif // !_declare_h_
