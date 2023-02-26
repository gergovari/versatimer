#ifndef HandlerManager_H
#define HandlerManager_H

#include "../handler/Handler.h"
#include "../handler/timer/TimerHandler.h"
#include "../handler/routine/RoutineHandler.h"

#include "UIManager.h"
#include "BtnManager.h"
#include "AlarmManager.h"
#include "MenuManager.h"
#include "RoutineManager.h"

class HandlerManager: public MenuItemProvider {
	TimerHandler timer;
	RoutineHandler routineHandler;

	MenuItem* items[2] = { new MenuItem("Timer", &timer), new MenuItem("Routine", &routineHandler) };

	Handler* handler = nullptr;
	Handler* lastHandler = nullptr;
	
	bool isMenuBtn = false;
	bool inMenu = true;
	
	void setupBtns(BtnManager*);
	void handleBtns(BtnManager*, MenuManager*);

	public:
		MenuItem* getItem(int);
		void begin(MenuManager*);
		void tick(BtnManager*, UIManager*, AlarmManager*, MenuManager*, RoutineManager*);
};

#endif
