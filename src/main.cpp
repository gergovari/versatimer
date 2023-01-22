#include <Arduino.h>

#include "manager/BtnManager.h"
#include "manager/UIManager.h"
#include "manager/AlarmManager.h"
#include "manager/HandlerManager.h"

BtnManager btn;
UIManager ui;
AlarmManager alarm;

HandlerManager handler;

void setup() {
	Serial.begin(9600);
	ui.begin();
}

void loop() {
	btn.tick();
	handler.tick(&btn, &ui, &alarm);
}
