#include <Arduino.h>

#include "manager/BtnManager.h"
#include "manager/UIManager.h"
#include "manager/HandlerManager.h"

BtnManager btn;
UIManager ui;
HandlerManager handler(&btn);

void setup() {
	Serial.begin(9600);
	ui.begin();
}

void loop() {
	btn.tick();
	handler.tick(&ui);
}
