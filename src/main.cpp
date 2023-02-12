#include <Arduino.h>

#include "manager/BtnManager.h"
#include "manager/UIManager.h"
#include "manager/AlarmManager.h"
#include "manager/HandlerManager.h"
#include "manager/MenuManager.h"

BtnManager btn;
UIManager ui;
AlarmManager alarm;
MenuManager menu;

HandlerManager handler;

void setup() {
	Serial.begin(9600);
	ui.begin();
}

void loop() {
	Serial.println(F("test"));
	btn.tick();
	handler.tick(&btn, &ui, &alarm, &menu);
}
