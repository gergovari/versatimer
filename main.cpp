#include <Arduino.h>

#include "BtnHandler.h"
#include "UIHandler.h"

#include "src/timer/TimerHandler.h"

BtnHandler btn;
UIHandler UI;

TimerHandler timer;

void setup() {
	Serial.begin(9600);
	timer.setupBtns(&btn);
	UI.begin();
}

void loop() {
	btn.tick();
	timer.tick();
	UI.tick(&timer);
}
