#include "AlarmHandler.h"

#include <Arduino.h>

AlarmHandler::AlarmHandler() {
	setState(false);
}

void AlarmHandler::updateAlarm() {
	pinMode(ALARM_PIN, OUTPUT);
	digitalWrite(ALARM_PIN, state);
}

void AlarmHandler::setState(bool newState) {
	state = newState;
	updateAlarm();
}
