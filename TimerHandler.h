#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include <Arduino.h>

enum MULT_STATE {
	SEC,
	MIN,
	HOUR
};

class TimerHandler {
	unsigned long startMillis;
	bool setStart;
	signed long curMult;
	unsigned long idleStart;
	unsigned long idleOffset;

	public:
		unsigned long target;
		unsigned long passed;

		TimerHandler();
		MULT_STATE getMultState();
		void advanceMult();
		bool isSetupFinished();
		void addToTarget(signed int sign);
		void tick();
		void startIdle();
		void stopIdle();
};

#endif
