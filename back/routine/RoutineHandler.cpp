#include "RoutineHandler.h"

void RoutineHandler::toggleState() {
	switch (state) {
		case SET_RUNNING: {
			if (timer.target == 0) {
				state = SET_DONE;

			} else {
				state = SET_IDLE;
				timer.startIdle();
			}
			break;
		}
		case SET_IDLE: {
			state = SET_RUNNING;
			timer.stopIdle();
			break;
		}
		case SET_DONE: {
			state = SET_RECORD;
			break;
		}
		case SET_RECORD: {
			break;
		}
		default: {
			break;
		}
	}
}

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
			((RoutineHandler*)(state)) -> toggleState();
		}, this
	);
}



Routine* routines[0];

bool RoutineHandler::tickSet(UIManager* ui, AlarmManager* alarm, Set* set) {
	timer.target = set -> duration;
	switch (state) {
		case SET_RUNNING: {
			if (timer.target != 0) {
				ui -> printRunning(&(timer.target), &(timer.passed));
				if (timer.tick()) {
					state = SET_DONE;
				}
			}
			ui -> printMsg(set -> name, false);
			alarm -> setState(false);
			break;
		}
		case SET_IDLE: {
			ui -> printRunning(&(timer.target), &(timer.passed), hasReturned);
			ui -> printMsg(set -> name, false);
			alarm -> setState(false);
			break;
		}
		case SET_DONE: {
			ui -> printAlarm(&(timer.target), &(timer.passed));
			ui -> printMsg(set -> name, true);
			if (timer.target != 0) {
				alarm -> setState(true);
			} else {
				state = SET_RECORD;
			}
			break;	    
		}
		case SET_RECORD: {
			timer = Timer();
			state = SET_RUNNING;
			ui -> clear();
			return true;
			break;	    
		}
		default: {
			break;
		}
	}
	return false;
}

unsigned int i = 0;
bool RoutineHandler::tickRoutine(UIManager* ui, AlarmManager* alarm, Routine* routine) {
	if (i < routine -> setCount) {
		if (tickSet(ui, alarm, &(routine -> sets[i]))) {
			i++;
		}
		return false;
	} else {
		return true;
	}
}

Routine* routine;
void RoutineHandler::tick(UIManager* ui, AlarmManager* alarm) {
	if (isSelection) {
		routine = routines[0];
		isSelection = false;
	} else {
		if (tickRoutine(ui, alarm, routine)) {
			isSelection = true;
		}
	}
	hasReturned = false;
}
