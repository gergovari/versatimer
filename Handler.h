#ifndef HANDLER_H
#define HANDLER_H

#include "BtnManager.h"
#include "UIManager.h"

class Handler {
	public:
		virtual void setupBtns(BtnManager*) {};
		virtual void tick(UIManager*) {};
};

#endif
