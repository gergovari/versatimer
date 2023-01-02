#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include <Arduino.h>

class TimerHandler {
	unsigned long startMillis;
	bool setStart;
	signed long curMult;
	bool isIdleStarted;
	unsigned long idleStart;
	public:
		volatile unsigned long target;
		signed long passed;

		TimerHandler();
		void advanceMult();
		bool isSetupFinished();
		void addToTarget(signed int sign);
		void tickIdle();
		void tickTimer();
};

#endif
