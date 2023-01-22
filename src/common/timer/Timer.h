#ifndef Timer_H
#define Timer_H

#include <Arduino.h>

// TODO: should this be included here? So far only TimerHandler uses it...
enum MULT_STATE {
	SEC,
	MIN,
	HOUR
};

class Timer {
	unsigned long startMillis = 0;
	bool setStart = false;
	unsigned long idleStart = 0;
	unsigned long idleOffset = 0;

	public:
		unsigned long target = 0;
		unsigned long passed = 0;

		MULT_STATE getMultState();
		bool tick();
		void startIdle();
		void stopIdle();

		// TODO: should this be included here? So far only TimerHandler uses it...
		signed long curMult = 1000;
		void advanceMult();
		bool isSetupFinished();
		void addToTarget(signed int sign);
};

#endif
