#include <SD.h>
#include "MenuManager.h"

#ifndef RoutineManager_H
#define RoutineManager_H

#define CHIP_SELECT 10

class Routine {
	public:
		unsigned int setCount = 0;
};

class RoutineManager: public MenuItemProvider {
	int currentPos = -1;

	File root = SD.open("/VERSAT~1/ROUTINES");
	File currentFile;
	MenuItem currentItem = MenuItem(nullptr, nullptr);
	
	void rewindPos();
	void advancePos();

	public:
		bool begin();
		MenuItem* getItem(int pos);
};

#endif
