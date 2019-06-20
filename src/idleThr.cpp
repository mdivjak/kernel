#include "../h/idleThr.h"

IdleThread::IdleThread() : Thread(512, 1) {}

void IdleThread::run() {
	while(1) {}
}