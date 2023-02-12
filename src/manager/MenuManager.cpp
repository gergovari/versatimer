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
	selection = sanitizeSelection(lastCount);
}

signed int MenuManager::sanitizeSelection(int count) {
	if (selection < 0) {
		return count - 1;
	} else if (selection >= count) {
		return 0;
	} else {
		return selection;
	}
	return 0;
}

void* MenuManager::showSelection(UIManager* ui, MenuItem* items[], int count) {
	const char* names[count];
	for (int i = 0; i < count; i++) {
		names[i] = items[i] -> name;
	}
	lastCount = count;
	ui -> printMenu(names, count, selection, !isMenuPrinted);
	isMenuPrinted = true;
	if (isSelected) {
		return items[sanitizeSelection(count)] -> item;
		isSelected = false;
	} else {
		return nullptr;
	}
}
