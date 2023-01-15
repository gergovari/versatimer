#ifndef HANDLER_H
#define HANDLER_H

#include "BtnHandler.h"
#include "UIHandler.h"

class Handler {
	public:
		virtual void setupBtns(BtnHandler*) {};
		virtual void tick(UIHandler*) {};
};

#endif
