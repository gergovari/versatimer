#include <Arduino.h>

#include "manager/BtnManager.h"
#include "manager/UIManager.h"

#include "handler/Handler.h"
#include "handler/timer/TimerHandler.h"

BtnManager btn;
UIManager ui;

TimerHandler handler;

void setup() {
	Serial.begin(9600);
	handler.setupBtns(&btn);
	ui.begin();
}

void loop() {
	btn.tick();
	handler.tick(&ui);
}
