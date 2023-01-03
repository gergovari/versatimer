#include <Arduino.h>

#include "StateHandler.h"
#include "UIHandler.h"

StateHandler state;
UIHandler UI;

void setup() {
	Serial.begin(9600);
	UI.begin();
}

void loop() {
	state.tickState();
	UI.tick(&state);
}
