#ifndef ALARMHANDLER_H
#define ALARMHANDLER_H

#include <Arduino.h>

#define ALARM_PIN 3

class AlarmHandler {
	void updateAlarm();
	public:
		bool state = false;
		AlarmHandler();
		void setState(bool newState);
};

#endif
