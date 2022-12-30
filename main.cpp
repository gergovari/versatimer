#include <Arduino.h>

#include "StateHandler.h"

StateHandler state;

void setup() {
	Serial.begin(9600);
}

void loop() {
	state.tickState();
}
