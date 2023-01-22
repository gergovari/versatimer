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

class RoutineHandler: public Handler {
	RoutineState state = SET_RUNNING;
	Timer timer;

	public:
		void setupBtns(BtnManager*);
		void tick(UIManager*, AlarmManager*);
};

#endif
