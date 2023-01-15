#ifndef ALARMHANDLER_H
#define ALARMHANDLER_H

#define ALARM_PIN 3

class AlarmManager {
	void updateAlarm();
	public:
		bool state = false;
		AlarmManager();
		void setState(bool newState);
};

#endif
