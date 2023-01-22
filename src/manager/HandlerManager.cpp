#include "HandlerManager.h"

void HandlerManager::handleBtns(BtnManager* btn) {
	if (lastHandler != handler) {
		handler -> setupBtns(btn);
		lastHandler = handler;
	}
}

void HandlerManager::tick(BtnManager* btn, UIManager* ui, AlarmManager* alarm) {
	handleBtns(btn);
	handler -> tick(ui, alarm);
}
