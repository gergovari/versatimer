#include "HandlerManager.h"

void HandlerManager::setupBtns(BtnManager* btn) {
	btn -> leftBtn.attachLongPressStart(
		[](void *state){
			// NOTE: simply setting handler to nullptr crashes the arduino...
			(*((HandlerManager*)state)).inMenu = true;
			(*((HandlerManager*)state)).isMenuBtn = false;
		}, this
	);
}

void HandlerManager::handleBtns(BtnManager* btn, MenuManager* menu) {
	if (lastHandler != handler) {
		handler -> setupBtns(btn);
		lastHandler = handler;
		setupBtns(btn);
	} else if (!isMenuBtn) {
		isMenuBtn = true;
		menu -> setupBtns(btn);
	}
}

void HandlerManager::tick(BtnManager* btn, UIManager* ui, AlarmManager* alarm, MenuManager* menu) {
	menu -> isMenuPrinted = isMenuBtn;
	handleBtns(btn, menu);
	if (inMenu) {
		void* item = (menu -> showSelection(ui, items, sizeof(items)/sizeof(items[0])));
		if (item) {
			ui -> clear();
			handler = (Handler*)item;
			inMenu = false;
		}
	} else {
		if (handler) {
			handler -> tick(ui, alarm);
		}
	}
}
