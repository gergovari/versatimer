#ifndef MenuManager_H
#define MenuManager_H

#include "UIManager.h"
#include "BtnManager.h"

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
		bool isSelected = false;
		int selection = 0;
		int lastCount = 0;

		void moveSelection(signed int);
		signed int sanitizeSelection(int);
		void* showSelection(UIManager*, MenuItem*[], int);
		void setupBtns(BtnManager*);
};

#endif
