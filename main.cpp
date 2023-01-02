#include <Arduino.h>
#include <LCD_I2C.h>

#include "StateHandler.h"
#include "State.h"

StateHandler state;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
LCD_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);

void setup() {
	Serial.begin(9600);
	lcd.begin();
	lcd.backlight();
	//lcd.noBacklight();
	lcd.setCursor(0, 0);
}

void timeToText(long* time, char *out, bool blinkHour = false, bool blinkMin = false, bool blinkSec = false) {
	long hour = (*time / 1000 / 60 / 60);
	long min = ((*time - (hour * 1000 * 60 * 60)) / 1000 / 60);
	long sec = ((*time - (hour * 1000 * 60 * 60) - (min * 1000 * 60)) / 1000);
	
	const int HOUR_FORMAT_SIZE = 6;
	const int MIN_FORMAT_SIZE = 6;
	const int SEC_FORMAT_SIZE = 6;
	char hourFormat[HOUR_FORMAT_SIZE];
	char minFormat[MIN_FORMAT_SIZE];
	char secFormat[SEC_FORMAT_SIZE];
	if (!blinkHour) {
		strncpy(hourFormat, "%01lu", HOUR_FORMAT_SIZE);
	}	
	if (!blinkMin) {
		strncpy(minFormat, "%02lu", MIN_FORMAT_SIZE);
	}	
	if (!blinkSec) {
		strncpy(secFormat, "%02lu", SEC_FORMAT_SIZE);
	}
	
	const int FORMAT_SIZE = 21;
	char format[FORMAT_SIZE];
	snprintf(format, FORMAT_SIZE, "%s:%s:%s", hourFormat, minFormat, secFormat);

	char text[LCD_COLUMNS + 1];
	snprintf(text, LCD_COLUMNS + 1, format, hour, min, sec);
	strncpy(out, text, LCD_COLUMNS + 1);
}

bool ifVisibleChange(long *a, long *b) {
	char outA[LCD_COLUMNS + 1];
	timeToText(a, outA);
	char outB[LCD_COLUMNS + 1];
	timeToText(b, outB);
	return strcmp(outA, outB);
}

void printTime(long *time, bool blinkHour = false, bool blinkMin = false, bool blinkSec = false) {
	char text[LCD_COLUMNS + 1];
	timeToText(time, text, blinkHour, blinkMin, blinkSec);
	lcd.print(text);
}

unsigned long lastTarget = 1000;
unsigned long lastPassed = 1000;
void loop() {
	state.tickState();
	switch (state.state) {
		case SETUP: {
			if (ifVisibleChange(&lastTarget, &(state.timer.target))) {
				lcd.clear();
				printTime(&(state.timer.target));
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
		}
	}
}
