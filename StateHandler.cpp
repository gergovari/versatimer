#include "StateHandler.h"

StateHandler::StateHandler() {
	state = SETUP;
	setupBtns();
}

void StateHandler::setupBtns() {
	btn.setupBtns(
		[](void *state){(*((StateHandler*)state)).timer.addToTarget(-1);}, 
		[](void *state){(*((StateHandler*)state)).timer.addToTarget(1);},
		[](void *state){(*((StateHandler*)state)).toggleState();},
		this
	);
}

// NOTE: for some reason *this = StateHandler() doesn't work...
void StateHandler::reset() {
	timer = TimerHandler();
	alarm = AlarmHandler();
	state = SETUP;
}

void StateHandler::tickState() {
	btn.tickBtns();
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

void StateHandler::toggleState() {
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
