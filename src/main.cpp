#include <Arduino.h>

#include "manager/BtnManager.h"
#include "manager/UIManager.h"
#include "manager/AlarmManager.h"
#include "manager/HandlerManager.h"
#include "manager/MenuManager.h"
#include "manager/RoutineManager.h"

BtnManager btn;
UIManager ui;
AlarmManager alarm;
MenuManager menu;
RoutineManager routineManager;

HandlerManager handler;

void setup() {
	Serial.begin(9600);
	ui.begin();
	handler.begin(&menu);
	routineManager.begin();
}

void loop() {
	btn.tick();
	handler.tick(&btn, &ui, &alarm, &menu, &routineManager);
}
