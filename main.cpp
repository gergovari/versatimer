
#include <LCD_I2C.h>
#include "State.h"
#include "TimerHandler.h"
#include "StateHandler.h"



class UIHandler {
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

	unsigned long lastBlink = 0;
	bool wasBlink = true;

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
	public:
		// NOTE: on avr/arduino(?) beginning stuff like this in a constructor breaks...
		void begin();
		void tick(StateHandler*);
};
void UIHandler::begin() {
	lcd = new LCD_I2C(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
	lcd -> begin();
	lcd -> backlight();
	lcd -> setCursor(0, 0);
}

bool UIHandler::isBlinkNeeded() {
	return millis() - lastBlink >= BLINK_DELAY;
}

// NOTE: would not doing unnecessary calculations improve perf?
unsigned long UIHandler::timeToHour(unsigned long* time) {
	return *time / 1000 / 60 / 60;
}
unsigned long UIHandler::timeToMin(unsigned long* time) {
	return (*time - (timeToHour(time) * 1000 * 60 * 60)) / 1000 / 60;
}
unsigned long UIHandler::timeToSec(unsigned long* time) {
	return (*time - (timeToHour(time) * 1000 * 60 * 60) - (timeToMin(time) * 1000 * 60)) / 1000;
}

char* UIHandler::getFormat(bool blink, int width, int size, char* out) {
	if (blink) {
		memset(out, '\0', width + 1);
		memset(out, ' ', width);
	} else {
		char format[size];
		snprintf(format, size, "%%0%dlu", width);
		strncpy(out, format, size);
	}
	return out;
}
char* UIHandler::getHourFormat(bool blink, char* out) {
	return getFormat(blink, 1, HOUR_FORMAT_SIZE, out);
}
char* UIHandler::getMinFormat(bool blink, char* out) {
	return getFormat(blink, 2, MIN_FORMAT_SIZE, out);
}
char* UIHandler::getSecFormat(bool blink, char* out) {
	return getFormat(blink, 2, SEC_FORMAT_SIZE, out);
}
char* UIHandler::getTimeFormat(bool blinkHour, bool blinkMin, bool blinkSec, char* out) {
	char hourOut[HOUR_FORMAT_SIZE];
	char minOut[MIN_FORMAT_SIZE];
	char secOut[SEC_FORMAT_SIZE];
	snprintf(
		out, 
		FORMAT_SIZE, 
		"%s:%s:%s", 
		getHourFormat(blinkHour, hourOut),
		getMinFormat(blinkMin, minOut), 
		getSecFormat(blinkSec, secOut)
	);
	return out;
}

char* UIHandler::getTimeText(unsigned long *time, bool blinkHour, bool blinkMin, bool blinkSec, char* out) {
	char format[FORMAT_SIZE];
	getTimeFormat(blinkHour, blinkMin, blinkSec, format);

	unsigned long hour = timeToHour(time);
	unsigned long min = timeToMin(time);
	unsigned long sec = timeToSec(time);

	// TODO: refactor
	// NOTE: WHERE IS %x$ ON AVR???? anyway... ugly ifs ahead
	// if you know a better solution, or know how to get indexing to work here... make a PR please
	if (blinkHour) {
		if (blinkMin) {
			if (blinkSec) {
				snprintf(out, LCD_COLUMNS + 1, format);
			} else {
				snprintf(out, LCD_COLUMNS + 1, format, sec);
			}
		} else {
			if (blinkSec) {
				snprintf(out, LCD_COLUMNS + 1, format, min);
			} else {
				snprintf(out, LCD_COLUMNS + 1, format, min, sec);
			}
		}
	} else if (blinkMin) {
		if (blinkHour) {
			if (blinkSec) {
				snprintf(out, LCD_COLUMNS + 1, format);
			} else {
				snprintf(out, LCD_COLUMNS + 1, format, sec);
			}
		} else {
			if (blinkSec) {
				snprintf(out, LCD_COLUMNS + 1, format, hour);
			} else {
				snprintf(out, LCD_COLUMNS + 1, format, hour, sec);
			}
		}
	} else if (blinkSec) {
		if (blinkHour) {
			if (blinkMin) {
				snprintf(out, LCD_COLUMNS + 1, format);
			} else {
				snprintf(out, LCD_COLUMNS + 1, format, min);
			}
		} else {
			if (blinkMin) {
				snprintf(out, LCD_COLUMNS + 1, format, hour);
			} else {
				snprintf(out, LCD_COLUMNS + 1, format, hour, min);
			}
		}
	} else {
		snprintf(out, LCD_COLUMNS + 1, format, hour, min, sec);
	}
	// forgive me...
	
	return out;
}

char* UIHandler::timeToText(
	unsigned long* time, 
	char *out, 
	bool blinkHour = false, 
	bool blinkMin = false, 
	bool blinkSec = false
) {
	char text[LCD_COLUMNS + 1];
	strncpy(out, getTimeText(time, blinkHour, blinkMin, blinkSec, text), LCD_COLUMNS + 1);
	return out;
}

bool UIHandler::ifVisibleChange(unsigned long *a, unsigned long *b, bool blink = false) {
	char outA[LCD_COLUMNS + 1];
	char outB[LCD_COLUMNS + 1];
	return strcmp(timeToText(a, outA), timeToText(b, outB)) || (blink && isBlinkNeeded());
}

bool UIHandler::handleBlink(bool blinkHour, bool blinkMin, bool blinkSec) {
	bool blink = false;
	if (blinkHour || blinkMin || blinkSec) {
		if (isBlinkNeeded()) {
			blink = !wasBlink;
			wasBlink = !wasBlink;
			lastBlink = millis();
		}
	}
	return blink;
}

void UIHandler::printTime(unsigned long *time, bool blinkHour = false, bool blinkMin = false, bool blinkSec = false) {
	bool blink = handleBlink(blinkHour, blinkMin, blinkSec);
	char text[LCD_COLUMNS + 1];
	lcd -> print(timeToText(time, text, blinkHour && blink, blinkMin && blink, blinkSec && blink));
}

void UIHandler::tick(StateHandler* state) {
	MULT_STATE multState = state -> timer.getMultState();
	bool hour = multState == HOUR;
	bool min = multState == MIN;
	bool sec = multState == SEC;

	switch (state -> state) {
		case SETUP: {
			if (ifVisibleChange(&lastTarget, &(state -> timer.target), true)) {
				lcd -> clear();
				printTime(&(state -> timer.target), hour, min, sec);
				lastTarget = state -> timer.target;
			}
			break;
		}
		case RUNNING: {
			if (ifVisibleChange(&lastPassed, &(state -> timer.passed)) || ifVisibleChange(&lastTarget, &(state -> timer.target))) {
				lcd -> clear();
				printTime(&(state -> timer.passed));
				lcd -> print(F("/"));
				printTime(&(state -> timer.target));
				lastPassed = state -> timer.passed;
				lastTarget = state -> timer.target;
			}
			break;
		}
		case ALARM: {
			if (ifVisibleChange(0, 0, true)) {
				lcd -> clear();
				printTime(&(state -> timer.passed), true, true, true);
				lcd -> print(F("/"));
				printTime(&(state -> timer.target));
			}
		}
		default: {
			break;	 
		}
	}
}

#include <Arduino.h>

#include "StateHandler.h"
#include "UIHandler.h"

StateHandler state;
UIHandler UI;

void setup() {
	Serial.begin(9600);
	UI.begin();
}

void loop() {
	state.tickState();
	UI.tick(&state);
}
