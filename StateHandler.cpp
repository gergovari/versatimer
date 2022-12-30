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

void StateHandler::tickState() {
	btn.tickBtns();
	switch (state) {
		case RUNNING: {
			signed long passed = timer.tickTimer();
			if (passed <= 0) {
				state = ALARM;
			}
			break;
		}
		case IDLE: {
			timer.tickIdle();
			break;
		}
		case ALARM: {
			alarm.setState(true);
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
			break;
		}
		case IDLE: {
			state = RUNNING;
			break;
		}
		case ALARM: {
			*this = StateHandler();
			break;
		}
	}
}
