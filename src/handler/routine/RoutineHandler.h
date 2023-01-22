#ifndef RoutineHandler_H
#define RoutineHandler_H

#include "../Handler.h"

enum RoutineState {
	SET_RUNNING,
	SET_DONE,
	SET_RECORD
};

class RoutineHandler: public Handler {
	RoutineState state = SET_RUNNING;

	public:
		void setupBtns(BtnManager*);
		void tick(UIManager*, AlarmManager*);
};

#endif
