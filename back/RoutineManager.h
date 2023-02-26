#include <SD.h>

#ifndef RoutineManager_H
#define RoutineManager_H

#define CHIP_SELECT 10

class RoutineManager {
	public:
		bool begin();
		Routine[]* getRoutines();
		
	Routine currentRoutines[];
};

#endif
