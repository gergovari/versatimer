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
		void setupBtns() {
			btn.setupBtns(
				[](void *state){(*((StateHandler*)state)).timer.addToTarget(-1);}, 
				[](void *state){(*((StateHandler*)state)).timer.addToTarget(1);},
				[](void *state){(*((StateHandler*)state)).toggleState();},
				this
			);
		}
		void toggleState() {
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
};

StateHandler state;

void setup() {
	Serial.begin(9600);
	state.setupBtns();
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
