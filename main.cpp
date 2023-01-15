#include <Arduino.h>

#include "BtnHandler.h"
#include "UIHandler.h"

#include "Handler.h"
#include "src/timer/TimerHandler.h"

BtnHandler btn;
UIHandler ui;

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
