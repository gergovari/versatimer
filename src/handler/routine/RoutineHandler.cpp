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


Set brk = { "Break", 0 };
Routine testRoutine = {
	"Test routine",
	{
		{
			"Test 1", 0, { {"REP", 12}, {"WGT", 6} }
		}, brk,
		{
			"Test 2", 0, { {"REP", 5}, {"WGT", 80} }
		}, brk,
		{
			"Test 3", 0, { {"REP", 9}, {"WGT", 305} }
		}

	}
};

Routine* routine = &testRoutine;
int i = 0;
void RoutineHandler::tick(UIManager* ui, AlarmManager* alarm) {
	Set* set = &(routine -> sets[i]);
	switch (state) {
		case SET_RUNNING: {
			ui -> printMsg(set -> name);
			alarm -> setState(false);
			state = SET_DONE;
			break;
		}
		case SET_DONE: {
			ui -> printMsg(set -> name);
			//alarm -> setState(true);
			break;	    
		}
		case SET_RECORD: {
			i++;
			state = SET_RUNNING;
			break;	    
		}
		default: {
			break;
		}
	}
}
