#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include "../../Handler.h"
#include "../../BtnManager.h"
#include "../../UIManager.h"

#include "TimerState.h"
#include "Timer.h"

#include "../../AlarmHandler.h"

class TimerHandler: public Handler {
	AlarmHandler alarm;
	
	void tickState();
	void tickUI(UIManager*);

	void toggleState();
	void reset();

	public:
		TimerState state;
		Timer timer;
		
		void setupBtns(BtnManager*);
		void tick(UIManager*);
};

#endif
