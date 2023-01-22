#ifndef RoutineHandler_H
#define RoutineHandler_H

#include "../Handler.h"

#include "../../common/timer/Timer.h"

enum RoutineState {
	SET_RUNNING,
	SET_IDLE,
	SET_DONE,
	SET_RECORD
};

struct Record {
	char name[4];
	int value;
};
struct Set {
	char name[17];
	unsigned long duration;
	Record records[];
};
struct Routine {
	char name[16];
	unsigned int setCount;
	Set sets[];
};

class RoutineHandler: public Handler {
	RoutineState state = SET_RUNNING;
	Timer timer;

	bool isSelection = true;

	void toggleState();
	bool tickSet(UIManager*, AlarmManager*, Set*);
	bool tickRoutine(UIManager*, AlarmManager*, Routine*);

	public:
		void setupBtns(BtnManager*);
		void tick(UIManager*, AlarmManager*);
};

#endif
