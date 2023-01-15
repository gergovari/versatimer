#include "Timer.h"

Timer::Timer() {
	target = 0;
	passed = 0;
	startMillis = 0;
	setStart = false;
	curMult = 1000;
	idleStart = 0;
	idleOffset = 0;
}

MULT_STATE Timer::getMultState() {
	MULT_STATE state = SEC;
	if (curMult == (long)1000 * 60) {
		state = MIN;
	} else if (curMult == (long)1000 * 60 * 60) {
		state = HOUR;
	}
	return state;
}

void Timer::advanceMult() {
	curMult *= 60ul;
}

bool Timer::isSetupFinished() {
	return curMult == (long) 1000 * 60 * 60;
}

void Timer::addToTarget(signed int sign) {
	signed long milli = sign * curMult;
	if ((signed long) target + milli >= 0) {
		target += milli;
	}
}

void Timer::startIdle() {
	idleStart = millis();
}
void Timer::stopIdle() {
	idleOffset += millis() - idleStart;
}

void Timer::tick() {
	unsigned long currentMillis = millis();
	if (!setStart) {
		startMillis = currentMillis;
		setStart = true;
	}
	// NOTE: make sure we don't roll over
	if (passed > target) {
		passed = 0;
	} else {
		passed = (target + idleOffset) - (currentMillis - startMillis);
	}
}
