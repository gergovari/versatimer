#ifndef RoutineHandler_H
#define RoutineHandler_H

#include "../Handler.h"

#include "../../common/timer/Timer.h"
#include "../../manager/RoutineManager.h"

enum RoutineState {
	SET_RUNNING,
	SET_IDLE,
	SET_DONE,
	SET_RECORD
};

class RoutineHandler: public Handler {
	RoutineState state = SET_RUNNING;
	Timer timer;

	bool isSelection = true;

	void toggleState();
	//bool tickSet(UIManager*, AlarmManager*, Set*);
	bool tickRoutine(UIManager*, AlarmManager*, Routine*);
	
	char name[8] = "Routine";

	public:
		void setupBtns(BtnManager*);
		void tick(UIManager*, AlarmManager*, RoutineManager*);
};

#endif
