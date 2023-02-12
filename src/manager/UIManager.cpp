#include "UIManager.h"

void UIManager::begin() {
	lcd = new LCD_I2C(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
	lcd -> begin();
	lcd -> backlight();
	lcd -> setCursor(0, 0);
}

bool UIManager::isBlinkNeeded() {
	return millis() - lastBlink >= BLINK_DELAY;
}

// NOTE: would not doing unnecessary calculations improve perf?
unsigned long UIManager::timeToHour(unsigned long* time) {
	return *time / 1000 / 60 / 60;
}
unsigned long UIManager::timeToMin(unsigned long* time) {
	return (*time - (timeToHour(time) * 1000 * 60 * 60)) / 1000 / 60;
}
unsigned long UIManager::timeToSec(unsigned long* time) {
	return (*time - (timeToHour(time) * 1000 * 60 * 60) - (timeToMin(time) * 1000 * 60)) / 1000;
}

char* UIManager::getFormat(bool blink, int width, int size, char* out) {
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
char* UIManager::getHourFormat(bool blink, char* out) {
	return getFormat(blink, 1, HOUR_FORMAT_SIZE, out);
}
char* UIManager::getMinFormat(bool blink, char* out) {
	return getFormat(blink, 2, MIN_FORMAT_SIZE, out);
}
char* UIManager::getSecFormat(bool blink, char* out) {
	return getFormat(blink, 2, SEC_FORMAT_SIZE, out);
}
char* UIManager::getTimeFormat(bool blinkHour, bool blinkMin, bool blinkSec, char* out) {
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

char* UIManager::getTimeText(unsigned long *time, bool blinkHour, bool blinkMin, bool blinkSec, char* out) {
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

char* UIManager::timeToText(
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

bool UIManager::ifVisibleChangeStr(char *a, char *b, bool blink = false) {
	return strcmp(a, b) || (blink && isBlinkNeeded());
}
bool UIManager::ifVisibleChange(unsigned long *a, unsigned long *b, bool blink = false) {
	char outA[LCD_COLUMNS + 1];
	char outB[LCD_COLUMNS + 1];
	return ifVisibleChangeStr(timeToText(a, outA), timeToText(b, outB), blink);
}

// FIXME: allow multiple blinks (ID system or smt)
bool UIManager::handleBlink(bool blinkHour, bool blinkMin, bool blinkSec) {
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

void UIManager::clear() {
	lcd -> clear();
}
void UIManager::clearLine(unsigned int line) {
	lcd -> setCursor(0, line);
	for (int i = 0; i < LCD_COLUMNS; i++) {
		lcd -> print(F(" "));
	}
}

void UIManager::printTime(unsigned long *time, bool blinkHour = false, bool blinkMin = false, bool blinkSec = false) {
	bool blink = handleBlink(blinkHour, blinkMin, blinkSec);
	char text[LCD_COLUMNS + 1];
	lcd -> print(timeToText(time, text, blinkHour && blink, blinkMin && blink, blinkSec && blink));
}

void UIManager::printSetup(unsigned long *target, bool hour, bool min, bool sec) {
	if (ifVisibleChange(&lastTarget, target, true)) {
		clearLine(0);
		lcd -> setCursor(0, 0);
		printTime(target, hour, min, sec);
		lastTarget = *target;
	}
}

void UIManager::printRunning(unsigned long *target, unsigned long *passed, bool forceRefresh) {
	if (ifVisibleChange(&lastPassed, passed) || ifVisibleChange(&lastTarget, target) || forceRefresh) {
		clearLine(0);
		lcd -> setCursor(0, 0);
		printTime(passed);
		lcd -> print(F("/"));
		printTime(target);
		lastPassed = *passed;
		lastTarget = *target;
	}
}

void UIManager::printAlarm(unsigned long *target, unsigned long *passed) {
	if (ifVisibleChange(passed, passed, true)) {
		clearLine(0);
		lcd -> setCursor(0, 0);
		printTime(passed, true, true, true);
		lcd -> print(F("/"));
		printTime(target);
	}
}

void UIManager::printMsg(char* msg, bool blink = false) {
	if (ifVisibleChangeStr(msg, lastMsg, blink)) {
		clearLine(1);
		lcd -> setCursor(0, 1);
		if (!handleBlink(true, true, true)) {
			lcd -> print(msg);
		}
	}
	strncpy(lastMsg, msg, LCD_COLUMNS + 1);
}

#include <Arduino.h>
int lastSelected = 0;
void UIManager::printMenu(const char* names[], int count, int selected, bool isNewNames) {
	if (isNewNames || lastSelected != selected) {
		lastSelected = selected;
		int relSelected = selected;
		while (relSelected >= LCD_ROWS) {
			relSelected -= LCD_ROWS;
		}
		int start = selected - relSelected;
		lcd -> clear();
		for (int i = 0; i < LCD_ROWS; i++) {
			if (start + i < count) {
				lcd -> setCursor(0, i);
				lcd -> print(names[start + i]);
				if (selected == i) {
					lcd -> setCursor(15, i);
					lcd -> print("<");
				}
			}
		}
	}
}
