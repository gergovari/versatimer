#ifndef TimerHandler_H
#define TimerHandler_H

#include "../Handler.h"

#include "../../common/timer/Timer.h"

enum TimerState { SETUP, RUNNING, ALARM, IDLE };

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
