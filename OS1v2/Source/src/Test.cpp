#include "../h/thread.h"
#include "../h/PCB.h"
#include "../h/declare.h"

const int n = 2;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(int k): Thread(4096, k) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	for(int p = 0; p < 10; p++) {
		syncPrintf("N%d\n", p);
		for(int i = 0; i < 30000; i++)
			for(int j = 0; j < 30000; j++);
	}
}

void fa() {
	syncPrintf("A\n");
}

void fb() {
	syncPrintf("B\n");
}

void fc() {
	syncPrintf("C\n");
}


int userMain(int argc, char** argv)
{
	PCB::running->registerHandler(3, fb);
	PCB::running->registerHandler(3, fc);
	TestThread ta(2), tb(9), tc(12);
	ta.registerHandler(3, fa);
	ta.start();
	ta.signal(3);
	ta.swap(3, fb, fa);
	/*for(int k = 0; k < 6; k++) {
		for(int i = 0; i < 30000; i++) {
			for(int j = 0; j < 30000; j++) {
			}
		}
	}*/
	for(int x = 0; x < 16; x++) {
		for(int y = 0; y < 30000; y++) {
			for(int z = 0; z < 30000; z++) {
			}
		}
	}
	ta.unregisterAllHandlers(3);
	dispatch();
	ta.signal(3);
	return 0;
}

