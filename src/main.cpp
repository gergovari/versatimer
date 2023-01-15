#include <Arduino.h>

#include "manager/BtnManager.h"
#include "manager/UIManager.h"

#include "handler/Handler.h"
#include "handler/timer/TimerHandler.h"

BtnManager btn;
UIManager ui;

TimerHandler timer = TimerHandler();
Handler* handlers[] = { &timer };
Handler* handler = handlers[0];
void setupHandlers() {
	for (unsigned int i = 0; i < sizeof(handlers)/sizeof(handlers[0]); i++) {
		handlers[i] -> setupBtns(&btn);
	}
}

void setup() {
	Serial.begin(9600);
	setupHandlers();
	ui.begin();
}

void loop() {
	btn.tick();
	handler -> tick(&ui);
}
