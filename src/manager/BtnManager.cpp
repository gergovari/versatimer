#include "BtnManager.h"

BtnManager::BtnManager() : btns {&leftBtn, &rightBtn, &okBtn} {
	leftBtn = OneButton(LEFT_PIN, true, true);
	rightBtn = OneButton(RIGHT_PIN, true, true);
	okBtn = OneButton(OK_PIN, true, true);
}

void BtnManager::tick() {
	for (unsigned int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++) {
		btns[i] -> tick();
	}
}