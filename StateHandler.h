#ifndef STATEHANDLER_H
#define STATEHANDLER_H

#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"
#include "State.h"

class StateHandler {
	BtnHandler btn;
	TimerHandler timer;
	AlarmHandler alarm;
	State state;

	void setupBtns();
	void toggleState();

	public:
		StateHandler();
		void tickState();
};

#endif
