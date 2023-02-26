#include "HandlerManager.h"

void HandlerManager::setupBtns(BtnManager* btn) {
	btn -> okBtn.attachLongPressStart(
		[](void *state){
			// NOTE: simply setting handler/lastHandler to nullptr crashes the arduino or just plain doesnt work...
			(*((HandlerManager*)state)).inMenu = true;
			(*((HandlerManager*)state)).isMenuBtn = false;
		}, this
	);
}

void HandlerManager::handleBtns(BtnManager* btn, MenuManager* menu) {
	if (isMenuBtn && !inMenu) {
		handler -> setupBtns(btn);
		lastHandler = handler;
		setupBtns(btn);
		isMenuBtn = false;
	} else if (!isMenuBtn && inMenu) {
		menu -> setupBtns(btn);
		isMenuBtn = true;
	}
}

MenuItem* HandlerManager::getItem(int pos) {
	return items[pos];
}

void HandlerManager::tick(BtnManager* btn, UIManager* ui, AlarmManager* alarm, MenuManager* menu) {
	menu -> provider = this;
	menu -> isMenuPrinted = isMenuBtn;
	handleBtns(btn, menu);
	if (inMenu) {
		alarm -> setState(false);
		void* item = menu -> showSelection(ui);
		if (item) {
			ui -> clear();
			handler = (Handler*)item;
			handler -> hasReturned = true;
			inMenu = false;
		}
	} else {
		if (handler) {
			handler -> tick(ui, alarm);
		}
	}
}
