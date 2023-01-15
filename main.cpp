#include <Arduino.h>

#include "BtnManager.h"
#include "UIManager.h"

#include "Handler.h"
#include "src/timer/TimerHandler.h"

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
