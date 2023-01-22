#ifndef HANDLERMANAGER_H
#define HANDLERMANAGER_H

#include "../handler/Handler.h"
#include "../handler/timer/TimerHandler.h"

#include "UIManager.h"
#include "BtnManager.h"

class HandlerManager {
	Handler* handlers[1];
	TimerHandler timer;

	Handler* handler;
	public:
		explicit HandlerManager(BtnManager*);
		void tick(UIManager*);
};

#endif
