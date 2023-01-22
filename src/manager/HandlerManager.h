#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "../handler/Handler.h"
#include "../handler/timer/TimerHandler.h"
#include "../handler/routine/RoutineHandler.h"

#include "UIManager.h"
#include "BtnManager.h"
#include "AlarmManager.h"

class HandlerManager {
	Handler* handlers[2];
	TimerHandler timer;
	RoutineHandler routine;

	Handler* handler;
	public:
		explicit HandlerManager(BtnManager*);
		void tick(UIManager*, AlarmManager*);
};

#endif
