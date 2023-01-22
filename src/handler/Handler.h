#ifndef HANDLER_H
#define HANDLER_H

#include "../manager/BtnManager.h"
#include "../manager/UIManager.h"
#include "../manager/AlarmManager.h"

class Handler {
	public:
		virtual void setupBtns(BtnManager*) {};
		virtual void tick(UIManager*, AlarmManager*) {};
};

#endif
