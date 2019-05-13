#include "../h/thread.h"
#include "../h/declare.h"
#include "../h/semaphor.h"
#include <iostream.h>
#include <stdlib.h>

/*
 	 Test: Semafori sa spavanjem 4
*/

int t=-1;

const int n=15;

Semaphore sem(0);

class TestThread : public Thread {
private:
	char c;
public:
	TestThread(char c) : Thread() { this->c = c; }
	virtual ~TestThread() { waitToComplete(); }
	void run() {
		sem.wait(0);
		syncPrintf("%c\n", c);
	}
};

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	int i;
	TestThread **niz = new TestThread*[10];
	for(i = 0; i < 10; i++) {
		niz[i] = new TestThread('A'+i);
		niz[i]->start();
	}
	Semaphore cekanje(0);
	cekanje.wait(36*55);
	syncPrintf("sem.val = %d\n", sem.val());
	int ret = sem.signal(-5);
		syncPrintf("\nsem.signal(3) returned %d\n", ret);
	syncPrintf("sem.val = %d\n", sem.val());
	for(i = 0; i < 10; i++) {
		delete niz[i];
	}

	delete niz;
	niz = 0;
	return 0;
}


