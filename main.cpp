#include <Arduino.h>
#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"

#include "State.h"
class StateHandler {
	public:
		State state;
		StateHandler();
		int toggleState(TimerHandler*);
};

StateHandler::StateHandler() {
	state = SETUP;
}
StateHandler::toggleState(TimerHandler *timer) {
	switch (state) {
		case SETUP: {
			if (timer -> isSetupFinished()) {
				state = RUNNING;
			} else {
				timer -> advanceMult();
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
			//reset();
			break;
		}
	}
}

BtnHandler btn;
TimerHandler timer;
AlarmHandler alarm;
StateHandler state;

void reset() {
	timer = TimerHandler();
	alarm = AlarmHandler();
	state = StateHandler();
}

void setupBtns() {
	btn.setupBtns(
		[](){timer.addToTarget(-1);}, 
		[](){timer.addToTarget(1);},
		[](){state.toggleState(&timer);}
	);
}


void setup() {
	Serial.begin(9600);
	setupBtns();
}

void tickState() {
	switch (state.state) {
		case RUNNING: {
			signed long passed = timer.tickTimer();
			if (passed <= 0) {
				state.state = ALARM;
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
void loop() {
	btn.tickBtns();
	tickState();
}
