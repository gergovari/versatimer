#include "BtnHandler.h"

BtnHandler::BtnHandler(State *state_p) : btns({ &leftBtn, &rightBtn, &okBtn }) {
	state = state_p;
	leftBtn = OneButton(LEFT_PIN, true, true);
	rightBtn = OneButton(RIGHT_PIN, true, true);
	okBtn = OneButton(OK_PIN, true, true);
}	
void BtnHandler::setupBtns(void (*left)(void*), void (*right)(void*), void (*ok)(void*)) {
	/*leftBtn.attachClick([](){addToTarget(-1);}, state);
	rightBtn.attachClick([](){addToTarget(1);});
	okBtn.attachClick(, state);*/
	leftBtn.attachClick(left, state);
	rightBtn.attachClick(right, state);
	okBtn.attachClick(ok, state);
}
void BtnHandler::tickBtns() {
	for (unsigned int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++) {
		btns[i] -> tick();
	}
}
