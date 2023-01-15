#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include "../Handler.h"
#include "../BtnHandler.h"

#include "Timer.h"
#include "TimerState.h"

#include "../AlarmHandler.h"

class TimerHandler: public Handler {
	AlarmHandler alarm;

	void toggleState();
	void reset();

	public:
		TimerState state;
		Timer timer;
		
		void tick();
};

#endif
