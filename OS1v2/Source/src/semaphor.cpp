#include "../h/semaphor.h"
#include "../h/KerSem.h"
#include "../h/declare.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal(int n) {
	myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->val();
}
