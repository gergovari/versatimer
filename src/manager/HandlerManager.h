#ifndef HandlerManager_H
#define HandlerManager_H

#include "../handler/Handler.h"
#include "../handler/timer/TimerHandler.h"
#include "../handler/routine/RoutineHandler.h"

#include "UIManager.h"
#include "BtnManager.h"
#include "AlarmManager.h"

class HandlerManager {
	TimerHandler timer;
	RoutineHandler routine;

	Handler* handlers[2] = { &timer, &routine };
	Handler* handler = handlers[1];

	
	Handler* lastHandler;
	void handleBtns(BtnManager*);

	public:
		void tick(BtnManager*, UIManager*, AlarmManager*);
};

#endif
