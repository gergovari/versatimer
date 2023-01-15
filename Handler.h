#ifndef HANDLER_H
#define HANDLER_H

#include "BtnManager.h"
#include "UIHandler.h"

class Handler {
	public:
		virtual void setupBtns(BtnManager*) {};
		virtual void tick(UIHandler*) {};
};

#endif
