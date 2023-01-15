#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include "../Handler.h"
#include "../../manager/BtnManager.h"
#include "../../manager/AlarmManager.h"
#include "../../manager/UIManager.h"

#include "TimerState.h"
#include "Timer.h"


class TimerHandler: public Handler {
	AlarmManager alarm;
	
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
