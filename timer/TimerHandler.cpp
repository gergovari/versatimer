#include "TimerHandler.h"

void TimerHandler::setupBtns(BtnHandler* btn) {
	btn -> leftBtn.attachClick(
		[](void *state){
			if ((*((TimerHandler*)state)).state == SETUP) {
				(*((TimerHandler*)state)).timer.addToTarget(-1);
			}
		}, this
	);
	btn -> rightBtn.attachClick(
		[](void *state){
			if ((*((TimerHandler*)state)).state == SETUP) {
				(*((TimerHandler*)state)).timer.addToTarget(1);
			}
		}, this
	);
	btn -> okBtn.attachClick(
		[](void *state){
			(*((TimerHandler*)state)).toggleState();
		}, this
	);
}

void TimerHandler::reset() {
	timer = Timer();
	alarm = AlarmTimerHandler();
	state = SETUP;
}

void TimerHandler::tick() {
	switch (state) {
		case RUNNING: {
			timer.tick();
			// NOTE: remember, passed is UNSIGNED, can't be smaller than 0 (we roll over...)
			if (timer.passed == 0) {
				state = ALARM;
			}
			break;
		}
		case ALARM: {
			alarm.setState(true);
			break;
		}
		default: {
			break;	 
		}
	}
}

void TimerHandler::toggleState() {
	switch (state) {
		case SETUP: {
			if (timer.isSetupFinished()) {
				state = RUNNING;
			} else {
				timer.advanceMult();
			}
			break;
		}
		case RUNNING: {
			state = IDLE;
			timer.startIdle();
			break;
		}
		case IDLE: {
			state = RUNNING;
			timer.stopIdle();
			break;
		}
		case ALARM: {
			reset();
			break;
		}
	}
}
