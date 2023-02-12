#include "HandlerManager.h"

bool isMenuBtn = false;
Handler* lastHandler = NULL;
void HandlerManager::handleBtns(BtnManager* btn, MenuManager* menu) {
	if (lastHandler != handler) {
		handler -> setupBtns(btn);
		lastHandler = handler;
	} else if (!isMenuBtn) {
		isMenuBtn = true;
		menu -> setupBtns(btn);
	}
}

void HandlerManager::tick(BtnManager* btn, UIManager* ui, AlarmManager* alarm, MenuManager* menu) {
	handleBtns(btn, menu);
	if (handler != NULL) {
		handler -> tick(ui, alarm);
	} else {
		void* item = (menu -> showSelection(ui, items, sizeof(items)/sizeof(items[0])));
		if (item != NULL) {
			handler = (Handler*)item;
		}
	}
}
