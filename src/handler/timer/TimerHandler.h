#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include "../Handler.h"

#include "TimerState.h"
#include "Timer.h"


class TimerHandler: public Handler {
	void tickState(AlarmManager*);
	void tickUI(UIManager*);

	void toggleState();
	void reset();

	public:
		TimerState state;
		Timer timer;
		
		void setupBtns(BtnManager*);
		void tick(UIManager*, AlarmManager*);
};

#endif
