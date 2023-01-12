#ifndef BtnHandler_H
#define BtnHandler_H

#include "State.h"

#include <OneButton.h>

#define LEFT_PIN 4
#define RIGHT_PIN 5
#define OK_PIN 6
class BtnHandler {
	OneButton *btns[3] {};
	public:
		OneButton leftBtn;
		OneButton rightBtn;
		OneButton okBtn;

		BtnHandler();
		void tickBtns();
};

#endif
