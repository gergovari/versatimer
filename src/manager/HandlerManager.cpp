#include "HandlerManager.h"

void HandlerManager::handleBtns(BtnManager* btn) {
	if (lastHandler != handler) {
		handler -> setupBtns(btn);
		lastHandler = handler;
	}
}

void HandlerManager::tick(BtnManager* btn, UIManager* ui, AlarmManager* alarm, MenuManager* menu) {
	if (handler != NULL) {
		handleBtns(btn);
		handler -> tick(ui, alarm);
	} else {
		MenuItem* items[] = { new MenuItem("Timer", handlers[0]), new MenuItem("Routine", handlers[1]) };
		void* item = (menu -> showSelection(ui, items, sizeof(items)/sizeof(items[0])));
		if (item != NULL) {
			handler = (Handler*)item;
		}
	}
}
