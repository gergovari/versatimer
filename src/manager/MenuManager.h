#ifndef MenuManager_H
#define MenuManager_H

#include "UIManager.h"

struct MenuItem {
	const char* name;
	void* item;
	MenuItem(const char* itemName, void* itemPtr) {
		name = itemName;
		item = itemPtr;
	};
};

class MenuManager {
	public:
		void* showSelection(UIManager*, MenuItem*[], int);
};

#endif
