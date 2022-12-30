#include <Arduino.h>
#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"

#include "State.h"
class StateHandler {
	BtnHandler btn;
	TimerHandler timer;
	AlarmHandler alarm;
	State state;
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
	public:
		StateHandler() {
			state = SETUP;
			setupBtns();
		}
		void setupBtns() {
			btn.setupBtns(
				[](void *state){(*((StateHandler*)state)).timer.addToTarget(-1);}, 
				[](void *state){(*((StateHandler*)state)).timer.addToTarget(1);},
				[](void *state){(*((StateHandler*)state)).toggleState();},
				this
			);
		}
		void tickState() {
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
};

StateHandler state;

void setup() {
	Serial.begin(9600);
}

void loop() {
	state.tickState();
}
