#include <Arduino.h>
#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"

#include "State.h"
class StateHandler {
	public:
		BtnHandler btn;
		TimerHandler timer;
		AlarmHandler alarm;
		State state = SETUP;
};

StateHandler state;

void toggleState() {
	switch (state.state) {
		case SETUP: {
			if (state.timer.isSetupFinished()) {
				state.state = RUNNING;
			} else {
				state.timer.advanceMult();
			}
			break;
		}
		case RUNNING: {
			state.state = IDLE;
			break;
		}
		case IDLE: {
			state.state = RUNNING;
			break;
		}
		case ALARM: {
			//reset();
			break;
		}
	}
}


void reset() {
	state = StateHandler();
}

void setupBtns() {
	state.btn.setupBtns(
		[](){state.timer.addToTarget(-1);}, 
		[](){state.timer.addToTarget(1);},
		[](){toggleState();}
	);
}


void setup() {
	Serial.begin(9600);
	setupBtns();
}

void tickState() {
	switch (state.state) {
		case RUNNING: {
			signed long passed = state.timer.tickTimer();
			if (passed <= 0) {
				state.state = ALARM;
			}
			break;
		}
		case IDLE: {
			state.timer.tickIdle();
			break;
		}
		case ALARM: {
			state.alarm.setState(true);
			break;
		}
	}
}
void loop() {
	state.btn.tickBtns();
	tickState();
}
