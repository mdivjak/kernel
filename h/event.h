#ifndef _event_h_
#define _event_h_
#include "../h/IVTEntry.h"
#include "../h/declare.h"

#define PREPAREENTRY(num, flag)\
	void interrupt inter##num(...);\
	IVTEntry newEntry##num(num,inter##num);\
	void interrupt inter##num(...){\
		newEntry##num.signal();\
		if(flag==1)\
			newEntry##num.callOld();\
	}

class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};


#endif
