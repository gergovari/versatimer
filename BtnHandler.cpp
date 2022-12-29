#include "BtnHandler.h"

BtnHandler::BtnHandler(State *state_p) : btns({ &leftBtn, &rightBtn, &okBtn }) {
	state = state_p;
	leftBtn = OneButton(LEFT_PIN, true, true);
	rightBtn = OneButton(RIGHT_PIN, true, true);
	okBtn = OneButton(OK_PIN, true, true);
}	
void BtnHandler::setupBtns() {
	leftBtn.attachClick([](){addToTarget(-1);});
	rightBtn.attachClick([](){addToTarget(1);});
	okBtn.attachClick([](void *state){
		switch (*((State*)state)) {
			case SETUP: {
				if (cur_mult == (long) 1000 * 60 * 60) {
					*((State*)state) = RUNNING;
				} else {
					cur_mult *= 60ul;
				}
				break;
			}
			case RUNNING: {
				*((State*)state) = IDLE;
				break;
			}
			case IDLE: {
				*((State*)state) = RUNNING;
				break;
			}
			case ALARM: {
				reset();
				break;
			}
		}
	}, state);
}
void BtnHandler::tickBtns() {
	for (unsigned int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++) {
		btns[i] -> tick();
	}
}
