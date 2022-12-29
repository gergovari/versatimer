#include <Arduino.h>
#include "BtnHandler.h"
#include "State.h"

State state = SETUP;

class TimerHandler {
	volatile unsigned long target = 0;
	unsigned long startMillis = 0;
	bool setStart = false;
	signed long curMult = 1000;
	bool isIdleStarted = false;
	unsigned long idleStart = 0;

	public:
		void advanceMult() {
			curMult *= 60ul;
		}
		bool isSetupFinished() {
			return curMult == (long) 1000 * 60 * 60;
		}
		void addToTarget(signed int sign) {
			signed long milli = sign * curMult;
			if ((signed long) target + milli >= 0) {
				target += milli;
			}
		}
		void tickIdle() {
			if (isIdleStarted) {
				idleStart = millis();
			}
		}
		signed long tickTimer() {
			unsigned long currentMillis = millis();
			if (isIdleStarted) {
				target += currentMillis - idleStart;
				isIdleStarted = false;
			}
			if (!setStart) {
				startMillis = currentMillis;
				setStart = true;
			}
			return target - (currentMillis - startMillis);
		}
};

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
