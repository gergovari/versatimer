#include "RoutineHandler.h"

struct Record {
	char name[4];
	int value;
};
struct Set {
	char name[17];
	unsigned long duration;
	Record records[];
};
struct Routine {
	char name[16];
	unsigned int setCount;
	Set sets[];
};

void RoutineHandler::setupBtns(BtnManager* btn) {
	btn -> leftBtn.attachClick(
		[](void *state){
		}, this
	);
	btn -> rightBtn.attachClick(
		[](void *state){
		}, this
	);
	btn -> okBtn.attachClick(
		[](void *state){
			switch (((RoutineHandler*)(state)) -> state) {
				case SET_RUNNING: {
					if ((((RoutineHandler*)(state)) -> timer).target == 0) {
						((RoutineHandler*)(state)) -> state = SET_DONE;

					} else {
						((RoutineHandler*)(state)) -> state = SET_IDLE;
						(((RoutineHandler*)(state)) -> timer).startIdle();
					}
					break;
				}
				case SET_IDLE: {
					((RoutineHandler*)(state)) -> state = SET_RUNNING;
					(((RoutineHandler*)(state)) -> timer).stopIdle();
					break;
				}
				case SET_DONE: {
					((RoutineHandler*)(state)) -> state = SET_RECORD;
					break;
				}
				case SET_RECORD: {
					break;
				}
				default: {
					break;
				}
			}
		}, this
	);
}


Set brk = { "Break", (unsigned long)2 * 1000 };
Routine testRoutine = {
	"Test routine",
	5,
	{
		{
			"Test 1", 0, { {"REP", 12}, {"WGT", 6} }
		}, brk,
		{
			"Test 2", (unsigned long)5 * 1000
		}, brk,
		{
			"Test 3", 0, { {"REP", 9}, {"WGT", 305} }
		}

	}
};

Routine* routine = &testRoutine;

unsigned int i = 0;
void RoutineHandler::tick(UIManager* ui, AlarmManager* alarm) {
	if (i < routine -> setCount) {
		Set* set = &(routine -> sets[i]);
		timer.target = set -> duration;
		switch (state) {
			case SET_RUNNING: {
				if (timer.target != 0) {
					ui -> printRunning(&(timer.target), &(timer.passed));
					if (timer.tick()) {
						state = SET_DONE;
					}
				}
				ui -> printMsg(set -> name);
				alarm -> setState(false);
				break;
			}
			case SET_DONE: {
				ui -> printMsg(set -> name);
				if (timer.target != 0) {
					alarm -> setState(true);
				} else {
					state = SET_RECORD;
				}
				break;	    
			}
			case SET_RECORD: {
				timer = Timer();
				i++;
				state = SET_RUNNING;
				ui -> clear();
				break;	    
			}
			default: {
				break;
			}
		}
	} else {
		ui -> clear();
		alarm -> setState(false);
	}
}
