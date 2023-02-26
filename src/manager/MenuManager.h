#ifndef MenuManager_H
#define MenuManager_H

#include "UIManager.h"
#include "BtnManager.h"

#define LCD_ROWS 2

struct MenuItem {
	const char* name;
	void* item;
	MenuItem(const char* itemName, void* itemPtr) {
		name = itemName;
		item = itemPtr;
	};
};

class MenuItemProvider {
	public:
		virtual MenuItem* getItem(int);
};

class MenuManager {
	bool isSelected = false;
	int selection = 0;
	MenuItem* items[LCD_ROWS];	

	void moveSelection(signed int);
	signed int sanitizeSelection();
	
	public:
		MenuItemProvider* provider;
		bool isMenuPrinted = false;
		
		void getNeededItems();
		void* showSelection(UIManager*);
		void setupBtns(BtnManager*);
};

#endif
