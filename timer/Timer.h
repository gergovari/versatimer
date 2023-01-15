#ifndef TIMER_H
#define TIMER_H

enum MULT_STATE {
	SEC,
	MIN,
	HOUR
};

class Timer {
	unsigned long startMillis;
	bool setStart;
	signed long curMult;
	unsigned long idleStart;
	unsigned long idleOffset;

	public:
		unsigned long target;
		unsigned long passed;

		Timer();
		MULT_STATE getMultState();
		void advanceMult();
		bool isSetupFinished();
		void addToTarget(signed int sign);
		void tick();
		void startIdle();
		void stopIdle();
};

#endif
