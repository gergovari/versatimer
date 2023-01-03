#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"
#include "State.h"

class StateHandler {
	BtnHandler btn;
	AlarmHandler alarm;

	void setupBtns();
	void toggleState();
	void reset();

	public:
		State state;
		TimerHandler timer;
		StateHandler();
		void tickState();
};

#endif
