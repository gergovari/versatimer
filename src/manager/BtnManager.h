#ifndef BtnManager_H
#define BtnManager_H

#include <OneButton.h>

#define LEFT_PIN 4
#define RIGHT_PIN 5
#define OK_PIN 6
class BtnManager {
	OneButton *btns[3] {};
	public:
		OneButton leftBtn;
		OneButton rightBtn;
		OneButton okBtn;

		BtnManager();
		void tick();
};

#endif
