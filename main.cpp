#include <Arduino.h>
#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"

#include "State.h"
class StateHandler {
	//BtnHandler* btn;
	//TimerHandler* timer;
	public:
		State state;
		AlarmHandler* alarm;
		//StateHandler(BtnHandler *btn_, TimerHandler *timer_, AlarmHandler *alarm_) {
		//StateHandler(AlarmHandler *alarm_) {
		StateHandler() {
			state = SETUP;
		}
		
};

BtnHandler btn = BtnHandler();
TimerHandler timer;
//AlarmHandler alarm;
//State state = SETUP;
//StateHandler state = StateHandler(&btn, &timer, &alarm);
//StateHandler state = StateHandler(&alarm);
StateHandler state;

void reset() {
	timer = TimerHandler();
	//state = StateHandler(&btn, &timer, &alarm);
	//state = StateHandler(&alarm);
	state = StateHandler();
	//state = SETUP;
}

void setupBtns() {
	btn.setupBtns(
		[](void*){timer.addToTarget(-1);}, 
		[](void*){timer.addToTarget(1);},
		[](void *state_){
			switch (*((State*)state_)) {
				case SETUP: {
					if (timer.isSetupFinished()) {
						*((State*)state_) = RUNNING;
					} else {
						timer.advanceMult();
					}
					break;
				}
				case RUNNING: {
					*((State*)state_) = IDLE;
					break;
				}
				case IDLE: {
					*((State*)state_) = RUNNING;
					break;
				}
				case ALARM: {
					reset();
					break;
				}
			}
		},
		&state
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
			state.alarm -> setState(true);
			break;
		}
	}
}
void loop() {
	btn.tickBtns();
	tickState();
}
