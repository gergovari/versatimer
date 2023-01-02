#include <Arduino.h>
#include <LCD_I2C.h>

#include "StateHandler.h"
#include "State.h"

#include "TimerHandler.h"

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
	
	const int HOUR_FORMAT_SIZE = 8;
	const int MIN_FORMAT_SIZE = 8;
	const int SEC_FORMAT_SIZE = 8;
	char hourFormat[HOUR_FORMAT_SIZE] = " ";
	char minFormat[MIN_FORMAT_SIZE] = "  ";
	char secFormat[SEC_FORMAT_SIZE] = "  ";
	if (!blinkHour) {
		strncpy(hourFormat, "%01lu", HOUR_FORMAT_SIZE);
	}	
	if (!blinkMin) {
		strncpy(minFormat, "%02lu", MIN_FORMAT_SIZE);
	}	
	if (!blinkSec) {
		strncpy(secFormat, "%02lu", SEC_FORMAT_SIZE);
	}
	
	const int FORMAT_SIZE = 27;
	char format[FORMAT_SIZE];
	snprintf(format, FORMAT_SIZE, "%s:%s:%s", hourFormat, minFormat, secFormat);

	char text[LCD_COLUMNS + 1];
	snprintf(text, LCD_COLUMNS + 1, format, hour, min, sec);
	strncpy(out, text, LCD_COLUMNS + 1);
}

unsigned long lastBlink = 0;
const int BLINK_DELAY = 2 * 1000;
bool isBlinkNeeded() {
	return millis() - lastBlink >= BLINK_DELAY;
}

bool ifVisibleChange(long *a, long *b, bool blink = false) {
	char outA[LCD_COLUMNS + 1];
	timeToText(a, outA);
	char outB[LCD_COLUMNS + 1];
	timeToText(b, outB);
	return strcmp(outA, outB) || (blink && isBlinkNeeded());
}

bool wasBlink = true;
void printTime(long *time, bool blinkHour = false, bool blinkMin = false, bool blinkSec = false) {
	char text[LCD_COLUMNS + 1];
	
	bool blink = false;
	if (blinkHour || blinkMin || blinkSec) {
		if (isBlinkNeeded()) {
			blink = !wasBlink;
			wasBlink = !wasBlink;
			lastBlink = millis();
		}
	}
	timeToText(time, text, blinkHour && blink, blinkMin && blink, blinkSec && blink);

	lcd.print(text);
}

unsigned long lastTarget = 1000;
unsigned long lastPassed = 1000;
bool alarmPrinted = false;
void loop() {
	state.tickState();

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
	}
}
