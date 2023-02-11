#include "MenuManager.h"

void* MenuManager::showSelection(UIManager* ui, MenuItem* items[], int count) {
	const char* names[count];
	for (int i = 0; i < count; i++) {
		names[i] = items[i] -> name;
	}
	ui -> printMenu(names, count, 0);
	return NULL;
}
