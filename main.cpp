#include <OneButton.h>

volatile unsigned long target = 0;
signed long cur_mult = 1000;
void addToTarget(signed int sign) {
	signed long milli = sign * cur_mult;
	if ((signed long) target + milli >= 0) {
		target += milli;
	}
}
unsigned long startMillis = 0;
bool setStart = false;
unsigned long idleStart = 0;
bool isIdleStarted = false;
signed long tickTimer() {
	unsigned long currentMillis = millis();
	if (isIdleStarted) {
		target += currentMillis - idleStart;
		isIdleStarted = false;
	}
	if (!setStart) {
		startMillis = currentMillis;
		setStart = true;
	}
	return target - (currentMillis - startMillis);
}


#define ALARM_PIN 3
void setAlarmState(bool state) {
	digitalWrite(ALARM_PIN, state);
}

enum State { SETUP, RUNNING, ALARM, IDLE };
State state = SETUP;
void reset() {
	setAlarmState(false);
	state = SETUP;
	target = 0;
	cur_mult = 1000;
	startMillis = 0;
	setStart = false;
	idleStart = 0;
	isIdleStarted = false;
}


#define LEFT_PIN 4
#define RIGHT_PIN 5
#define OK_PIN 6
class BtnHandler {
	OneButton leftBtn = OneButton(LEFT_PIN, true, true);
	OneButton rightBtn = OneButton(RIGHT_PIN, true, true);
	OneButton okBtn = OneButton(OK_PIN, true, true);

	State *state;
	public:
		BtnHandler(State *state_p) {
			state = state_p;
		}	

	OneButton *btns[3] = { &leftBtn, &rightBtn, &okBtn };

	void tickBtns() {
		for (unsigned int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++) {
			btns[i] -> tick();
		}
	}
	void setupBtns() {
		leftBtn.attachClick([](){addToTarget(-1);});
		rightBtn.attachClick([](){addToTarget(1);});
		okBtn.attachClick([](void *ctx){
			switch (*(((BtnHandler*)(ctx)) -> state)) {
				case SETUP: {
					if (cur_mult == (long) 1000 * 60 * 60) {
						*(((BtnHandler*)(ctx)) -> state) = RUNNING;
					} else {
						cur_mult *= 60ul;
					}
					break;
				}
				case RUNNING: {
					*(((BtnHandler*)(ctx)) -> state) = IDLE;
					break;
				}
				case IDLE: {
					*(((BtnHandler*)(ctx)) -> state) = RUNNING;
					break;
				}
				case ALARM: {
					reset();
					break;
				}
			}
		}, this);
	}
};

void setPinModes() {
	pinMode(ALARM_PIN, OUTPUT);
	setAlarmState(false);
}

BtnHandler btn = BtnHandler(&state);
void setup() {
	Serial.begin(9600);
	setPinModes();
	btn.setupBtns();
}

void loop() {
	btn.tickBtns();
	switch (state) {
		case RUNNING: {
			signed long passed = tickTimer();
			if (passed <= 0) {
				state = ALARM;
			}
			break;
		}
		case IDLE: {
			if (!isIdleStarted) {
				idleStart = millis();
			}
			break;
		}
		case ALARM: {
			setAlarmState(true);
			break;
		}
	}
}
