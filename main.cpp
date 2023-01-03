#include <Arduino.h>
#include <LCD_I2C.h>

#include "StateHandler.h"
#include "State.h"

#include "TimerHandler.h"

StateHandler state;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
LCD_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

// NOTE: would not doing unnecessary calculations improve perf?
unsigned long timeToHour(unsigned long* time) {
	return *time / 1000 / 60 / 60;
}
unsigned long timeToMin(unsigned long* time) {
	return (*time - (timeToHour(time) * 1000 * 60 * 60)) / 1000 / 60;
}
unsigned long timeToSec(unsigned long* time) {
	return (*time - (timeToHour(time) * 1000 * 60 * 60) - (timeToMin(time) * 1000 * 60)) / 1000;
}

const int HOUR_FORMAT_SIZE = 5 + 1;
const int MIN_FORMAT_SIZE = 5 + 1;
const int SEC_FORMAT_SIZE = 5 + 1;
const int FORMAT_SIZE = HOUR_FORMAT_SIZE + MIN_FORMAT_SIZE + SEC_FORMAT_SIZE + 2 + 1;
char* getFormat(bool blink, int width, int size, char* out) {
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
char* getHourFormat(bool blink, char* out) {
	return getFormat(blink, 1, HOUR_FORMAT_SIZE, out);
}
char* getMinFormat(bool blink, char* out) {
	return getFormat(blink, 2, MIN_FORMAT_SIZE, out);
}
char* getSecFormat(bool blink, char* out) {
	return getFormat(blink, 2, SEC_FORMAT_SIZE, out);
}
char* getTimeFormat(bool blinkHour, bool blinkMin, bool blinkSec, char* out) {
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
char* getTimeText(unsigned long *time, bool blinkHour, bool blinkMin, bool blinkSec, char* out) {
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


char* timeToText(
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

unsigned long lastBlink = 0;
const int BLINK_DELAY = 500;
bool isBlinkNeeded() {
	return millis() - lastBlink >= BLINK_DELAY;
}
bool wasBlink = true;
bool handleBlink(bool blinkHour, bool blinkMin, bool blinkSec) {
	if (blinkHour || blinkMin || blinkSec) {
		if (isBlinkNeeded()) {
			blink = !wasBlink;
			wasBlink = !wasBlink;
			lastBlink = millis();
		}
	}
}

bool ifVisibleChange(unsigned long *a, unsigned long *b, bool blink = false) {
	char outA[LCD_COLUMNS + 1];
	char outB[LCD_COLUMNS + 1];
	return strcmp(timeToText(a, outA), timeToText(b, outB)) || (blink && isBlinkNeeded());
}

void printTime(unsigned long *time, bool blinkHour = false, bool blinkMin = false, bool blinkSec = false) {
	bool blink = handleBlink();
	char text[LCD_COLUMNS + 1];
	lcd.print(timeToText(time, text, blinkHour && blink, blinkMin && blink, blinkSec && blink););
}

unsigned long lastTarget = 1000;
unsigned long lastPassed = 1000;
void tickUI() {
	MULT_STATE multState = state.timer.getMultState();
	bool hour = multState == HOUR;
	bool min = multState == MIN;
	bool sec = multState == SEC;

	switch (state.state) {
		case SETUP: {
			if (ifVisibleChange(&lastTarget, &(state.timer.target), true)) {
				lcd.clear();
				printTime(&(state.timer.target), hour, min, sec);
				lastTarget = state.timer.target;
			}
			break;
		}
		case RUNNING: {
			if (ifVisibleChange(&lastPassed, &(state.timer.passed)) || ifVisibleChange(&lastTarget, &(state.timer.target))) {
				lcd.clear();
				printTime(&(state.timer.passed));
				lcd.print(F("/"));
				printTime(&(state.timer.target));
				lastPassed = state.timer.passed;
				lastTarget = state.timer.target;
			}
			break;
		}
		case ALARM: {
			if (ifVisibleChange(0, 0, true)) {
				lcd.clear();
				printTime(&(state.timer.passed), true, true, true);
				lcd.print(F("/"));
				printTime(&(state.timer.target));
			}
		}
		default: {
			break;	 
		}
	}
}

void setup() {
	Serial.begin(9600);
	lcd.begin();
	lcd.backlight();
	//lcd.noBacklight();
	lcd.setCursor(0, 0);
}

void loop() {
	state.tickState();
	tickUI();
}
