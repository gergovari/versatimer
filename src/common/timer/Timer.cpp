#include "Timer.h"

MULT_STATE Timer::getMultState() {
	MULT_STATE state = SEC;
	if (curMult == (long)1000 * 60) {
		state = MIN;
	} else if (curMult == (long)1000 * 60 * 60) {
		state = HOUR;
	}
	return state;
}

void Timer::startIdle() {
	idleStart = millis();
}
void Timer::stopIdle() {
	idleOffset += millis() - idleStart;
}

bool Timer::tick() {
	unsigned long currentMillis = millis();
	if (!setStart) {
		startMillis = currentMillis;
		setStart = true;
	}
	// NOTE: make sure we don't roll over
	if (passed > target || (target + idleOffset) < (currentMillis - startMillis)) {
		passed = 0;
	} else {
		passed = (target + idleOffset) - (currentMillis - startMillis);
	}
	return passed == 0;
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
