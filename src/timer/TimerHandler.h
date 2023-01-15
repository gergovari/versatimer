#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include "../../Handler.h"
#include "../../BtnManager.h"
#include "../../UIHandler.h"

#include "TimerState.h"
#include "Timer.h"

#include "../../AlarmHandler.h"

class TimerHandler: public Handler {
	AlarmHandler alarm;
	
	void tickState();
	void tickUI(UIHandler*);

	void toggleState();
	void reset();

	public:
		TimerState state;
		Timer timer;
		
		void setupBtns(BtnManager*);
		void tick(UIHandler*);
};

#endif
