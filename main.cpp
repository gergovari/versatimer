#include <Arduino.h>
#include "State.h"
#include "BtnHandler.h"
#include "TimerHandler.h"

State state = SETUP;

BtnHandler btn = BtnHandler(&state);
TimerHandler timer = TimerHandler();

#define ALARM_PIN 3
void setAlarmState(bool state) {
	digitalWrite(ALARM_PIN, state);
}

void reset() {
	setAlarmState(false);
	state = SETUP;
	timer = TimerHandler();
}

void setPinModes() {
	pinMode(ALARM_PIN, OUTPUT);
	setAlarmState(false);
}

void setup() {
	Serial.begin(9600);
	setPinModes();
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

void loop() {
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
			setAlarmState(true);
			break;
		}
	}
}
