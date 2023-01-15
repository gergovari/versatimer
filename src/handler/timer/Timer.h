#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

enum MULT_STATE {
	SEC,
	MIN,
	HOUR
};

class Timer {
	unsigned long startMillis = 0;
	bool setStart = false;
	signed long curMult = 1000;
	unsigned long idleStart = 0;
	unsigned long idleOffset = 0;

	public:
		unsigned long target = 0;
		unsigned long passed = 0;

		MULT_STATE getMultState();
		void advanceMult();
		bool isSetupFinished();
		void addToTarget(signed int sign);
		void tick();
		void startIdle();
		void stopIdle();
};

#endif
