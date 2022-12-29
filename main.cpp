#include <Arduino.h>
#include "BtnHandler.h"
#include "State.h"

State state = SETUP;

volatile unsigned long target = 0;
signed long cur_mult = 1000;
void addToTarget(signed int sign) {
	signed long milli = sign * cur_mult;
	if ((signed long) target + milli >= 0) {
		target += milli;
	}
}
unsigned long startMillis = 0;
bool setStart = false;
unsigned long idleStart = 0;
bool isIdleStarted = false;
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


#define ALARM_PIN 3
void setAlarmState(bool state) {
	digitalWrite(ALARM_PIN, state);
}

void reset() {
	setAlarmState(false);
	state = SETUP;
	target = 0;
	cur_mult = 1000;
	startMillis = 0;
	setStart = false;
	idleStart = 0;
	isIdleStarted = false;
}

void setPinModes() {
	pinMode(ALARM_PIN, OUTPUT);
	setAlarmState(false);
}

BtnHandler btn = BtnHandler(&state);
void setup() {
	Serial.begin(9600);
	setPinModes();
	btn.setupBtns();
}

void loop() {
	btn.tickBtns();
	switch (state) {
		case RUNNING: {
			signed long passed = tickTimer();
			if (passed <= 0) {
				state = ALARM;
			}
			break;
		}
		case IDLE: {
			if (!isIdleStarted) {
				idleStart = millis();
			}
			break;
		}
		case ALARM: {
			setAlarmState(true);
			break;
		}
	}
}
