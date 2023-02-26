#include "RoutineManager.h"

bool RoutineManager::begin() {
	return SD.begin(CHIP_SELECT);
}

void RoutineManager::advancePos() {
	currentFile = root.openNextFile();
	currentPos++;
}

void RoutineManager::rewindPos() {
	root.rewindDirectory();
	currentPos = -1;
}

MenuItem* RoutineManager::getItem(int pos) {
	if (currentPos > pos) {
		rewindPos();
	}
	while (currentPos < pos) {
		advancePos();
	}
	currentItem = MenuItem("test", &currentFile);
	return &currentItem;
}
