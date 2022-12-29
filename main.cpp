#include <Arduino.h>
#include "State.h"
#include "BtnHandler.h"
#include "TimerHandler.h"
#include "AlarmHandler.h"

State state = SETUP;

BtnHandler btn = BtnHandler(&state);
TimerHandler timer = TimerHandler();
AlarmHandler alarm = AlarmHandler();

void reset() {
	state = SETUP;
	timer = TimerHandler();
	alarm = AlarmHandler();
}

void setupBtns() {
	btn.setupBtns(
		[](void*){timer.addToTarget(-1);}, 
		[](void*){timer.addToTarget(1);},
		[](void *state){
			switch (*((State*)state)) {
				case SETUP: {
					if (timer.isSetupFinished()) {
						*((State*)state) = RUNNING;
					} else {
						timer.advanceMult();
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
		}
	);
}


void setup() {
	Serial.begin(9600);
	setupBtns();
}

void tickState() {
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
void loop() {
	btn.tickBtns();
	tickState();
}
