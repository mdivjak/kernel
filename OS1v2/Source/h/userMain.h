#ifndef _user_main_h_
#define _user_main_h_
#include "thread.h"

int userMain(int argc, char* argv[]);

class UserMain : public Thread {
private:
	int argc;
	char **argv;
public:
	UserMain(int argc, char **argv) : Thread(2048, 1) {
		this->argc = argc;
		this->argv = argv;
	}
protected:
	void run() {
		userMain(argc, argv);
	}
};

#endif // !_user_main_h_
