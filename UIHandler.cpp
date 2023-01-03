#include "UIHandler.h"

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
		// FIXME: cant redo setup, only work once
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
			if (ifVisibleChange(&(state -> timer.passed), &(state -> timer.passed), true)) {
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
