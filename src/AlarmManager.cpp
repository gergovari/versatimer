#include "AlarmManager.h"

#include <Arduino.h>

AlarmManager::AlarmManager() {
	setState(false);
}

void AlarmManager::updateAlarm() {
	pinMode(ALARM_PIN, OUTPUT);
	digitalWrite(ALARM_PIN, state);
}

void AlarmManager::setState(bool newState) {
	state = newState;
	updateAlarm();
}
