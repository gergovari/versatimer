#include "RoutineManager.h"

bool RoutineManager::begin() {
	return SD.begin(CHIP_SELECT);
}

Routine[]* RoutineManager::getRoutines() {
	File root = SD.open("/VERSAT~1/ROUTINES");
	while (true) {
		File entry = root.openNextFile();
		if (entry) {
			Serial.println(entry.name());
		} else {
			break;
		}
	}
}
