#include "TimerHandler.h"

TimerHandler::TimerHandler() {
	target = 0;
	startMillis = 0;
	setStart = false;
	curMult = 1000;
	isIdleStarted = false;
	idleStart = 0;
}

void TimerHandler::advanceMult() {
	curMult *= 60ul;
}

bool TimerHandler::isSetupFinished() {
	return curMult == (long) 1000 * 60 * 60;
}

void TimerHandler::addToTarget(signed int sign) {
	signed long milli = sign * curMult;
	if ((signed long) target + milli >= 0) {
		target += milli;
	}
}

void TimerHandler::tickIdle() {
	if (isIdleStarted) {
		idleStart = millis();
	}
}

signed long TimerHandler::tickTimer() {
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
