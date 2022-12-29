#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include <Arduino.h>

class TimerHandler {
	volatile unsigned long target;
	unsigned long startMillis;
	bool setStart;
	signed long curMult;
	bool isIdleStarted;
	unsigned long idleStart;
	public:
		TimerHandler();
		void advanceMult();
		bool isSetupFinished();
		void addToTarget(signed int sign);
		void tickIdle();
		signed long tickTimer();
};

#endif
