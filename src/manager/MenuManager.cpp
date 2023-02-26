#include "MenuManager.h"

void MenuManager::setupBtns(BtnManager* btn) {
	btn -> leftBtn.attachClick(
		[](void *state){
			(*((MenuManager*)state)).moveSelection(-1);
		}, this
	);
	btn -> rightBtn.attachClick(
		[](void *state){
			(*((MenuManager*)state)).moveSelection(1);
		}, this
	);
	btn -> okBtn.attachClick(
		[](void *state){
			(*((MenuManager*)state)).isSelected = true;
		}, this
	);
}

void MenuManager::moveSelection(signed int direction) {
	selection += direction;
	selection = sanitizeSelection();
	getNeededItems();
}

void MenuManager::getNeededItems() {
	int relSelection = selection;
	while (relSelection >= LCD_ROWS) {
		relSelection -= LCD_ROWS;
	}
	int start = selection - relSelection;
	for (int i = 0; i < LCD_ROWS; i++) {
		items[i] = provider -> getItem(start + i);
	}
}

signed int MenuManager::sanitizeSelection() {
	if (selection < 0) {
		//return count - 1;
		return 0;
	/*} else if (selection >= count) {
		return 0;*/
	} else {
		return selection;
	}
}

void* MenuManager::showSelection(UIManager* ui) {
	getNeededItems();
	const char* names[LCD_ROWS];
	for (int i = 0; i < LCD_ROWS; i++) {
		names[i] = items[i] -> name;
	}
	ui -> printMenu(names, selection, !isMenuPrinted);
	if (isSelected) {
		isSelected = false;
		return items[sanitizeSelection()] -> item;
	} else {
		return nullptr;
	}
}
