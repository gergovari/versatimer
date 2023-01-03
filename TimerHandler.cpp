#include "TimerHandler.h"

TimerHandler::TimerHandler() {
	target = 0;
	passed = 0;
	startMillis = 0;
	setStart = false;
	curMult = 1000;
	idleStart = 0;
	idleOffset = 0;
}

MULT_STATE TimerHandler::getMultState() {
	MULT_STATE state = SEC;
	if (curMult == (long)1000 * 60) {
		state = MIN;
	} else if (curMult == (long)1000 * 60 * 60) {
		state = HOUR;
	}
	return state;
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

void TimerHandler::startIdle() {
	idleStart = millis();
}
void TimerHandler::stopIdle() {
	idleOffset -= millis() - idleStart;
}

void TimerHandler::tick() {
	unsigned long currentMillis = millis();
	if (!setStart) {
		startMillis = currentMillis;
		setStart = true;
	}
	// NOTE: make sure we don't roll over
	if (passed > target || (currentMillis - startMillis - idleOffset) > target) {
		passed = 0;
	} else {
		passed = target - (currentMillis - startMillis) - idleOffset;
	}
}
