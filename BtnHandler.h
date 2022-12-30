#ifndef BtnHandler_H
#define BtnHandler_H

#include "State.h"

#include <OneButton.h>

#define LEFT_PIN 4
#define RIGHT_PIN 5
#define OK_PIN 6
class BtnHandler {
	OneButton leftBtn;
	OneButton rightBtn;
	OneButton okBtn;
	OneButton *btns[3] {};

	public:
		BtnHandler();

	void setupBtns(void (*)(void*), void (*)(void*), void (*)(void*), void*);
	void tickBtns();
};

#endif
