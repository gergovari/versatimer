#include "TimerHandler.h"

void TimerHandler::setupBtns(BtnManager* btn) {
	btn -> leftBtn.attachClick(
		[](void *state){
			if ((*((TimerHandler*)state)).state == SETUP) {
				(*((TimerHandler*)state)).timer.addToTarget(-1);
			}
		}, this
	);
	btn -> rightBtn.attachClick(
		[](void *state){
			if ((*((TimerHandler*)state)).state == SETUP) {
				(*((TimerHandler*)state)).timer.addToTarget(1);
			}
		}, this
	);
	btn -> okBtn.attachClick(
		[](void *state){
			(*((TimerHandler*)state)).toggleState();
		}, this
	);
}

void TimerHandler::reset() {
	timer = Timer();
	state = SETUP;
}

void TimerHandler::tickState(AlarmManager* alarm) {
	switch (state) {
		case SETUP: {
			alarm -> setState(false);
			break;	    
		}
		case RUNNING: {
			if (timer.tick()) {
				state = ALARM;
			}
			break;
		}
		case ALARM: {
			alarm -> setState(true);
			break;
		}
		default: {
			break;	 
		}
	}
}

void TimerHandler::tickUI(UIManager* ui) {
	switch (state) {
		case SETUP: {
			MULT_STATE multState = timer.getMultState();
			ui -> printSetup(&(timer.target), multState == HOUR, multState == MIN, multState == SEC);
			break;
		}
		case RUNNING: {
			ui -> printRunning(&(timer.target), &(timer.passed));
			break;
		}
		case ALARM: {
			ui -> printAlarm(&(timer.target), &(timer.passed));
			break;
		}
		default: {
			break;	 
		}
	}
}

void TimerHandler::tick(UIManager* ui, AlarmManager* alarm) {
	tickState(alarm);
	tickUI(ui);
}

void TimerHandler::toggleState() {
	switch (state) {
		case SETUP: {
			if (timer.isSetupFinished()) {
				state = RUNNING;
			} else {
				timer.advanceMult();
			}
			break;
		}
		case RUNNING: {
			state = IDLE;
			timer.startIdle();
			break;
		}
		case IDLE: {
			state = RUNNING;
			timer.stopIdle();
			break;
		}
		case ALARM: {
			reset();
			break;
		}
	}
}
