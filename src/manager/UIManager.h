
#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <LCD_I2C.h>

class UIManager {
	const int LCD_COLUMNS = 16;
	const int LCD_ROWS = 2;
	const int LCD_ADDRESS = 0x27;
	LCD_I2C* lcd;

	const unsigned int BLINK_DELAY = 500;

	const int HOUR_FORMAT_SIZE = 5 + 1;
	const int MIN_FORMAT_SIZE = 5 + 1;
	const int SEC_FORMAT_SIZE = 5 + 1;
	const int FORMAT_SIZE = HOUR_FORMAT_SIZE + MIN_FORMAT_SIZE + SEC_FORMAT_SIZE + 2 + 1;

	// NOTE: arbitrary values, just don't make them 0 so there'll be a trigger to change the UI
	unsigned long lastTarget = 420;
	unsigned long lastPassed = 2121;
	char lastMsg[17];

	unsigned long lastBlink = 0;
	bool wasBlink = true;

	bool ifVisibleChangeStr(char*, char*, bool);
	bool ifVisibleChange(unsigned long*, unsigned long*, bool);
	bool isBlinkNeeded();
	unsigned long timeToHour(unsigned long*);
	unsigned long timeToMin(unsigned long*);
	unsigned long timeToSec(unsigned long*);
	char* getFormat(bool, int, int, char*);
	char* getHourFormat(bool, char*);
	char* getMinFormat(bool, char*);
	char* getSecFormat(bool, char*);
	char* getTimeFormat(bool, bool, bool, char*);
	char* getTimeText(unsigned long*, bool, bool, bool, char*);
	char* timeToText(unsigned long*, char*, bool, bool, bool);
	bool handleBlink(bool, bool, bool);
	void printTime(unsigned long*, bool, bool, bool);
	void clearLine(unsigned int);
	public:
		// NOTE: on avr/arduino(?) beginning stuff like this in a constructor breaks...
		void begin();

		void clear();

		void printSetup(unsigned long*, bool, bool, bool);
		void printRunning(unsigned long*, unsigned long*);
		void printAlarm(unsigned long*, unsigned long*);

		void printMsg(char*);
};

#endif
