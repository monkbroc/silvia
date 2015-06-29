#include "SlowPWM.h"
#include "application.h"

/* Ugly hack with globals cause I can't figure out how to use a C++ class object
 * in a C callback */
static SlowPWM *slowPwmInstance;

static void slowPwmCallback() {
    if(slowPwmInstance != NULL) {
        slowPwmInstance->handler();
    }
}

const unsigned short SlowPWM::COMPLETE_PERIOD = 1000; /* 100% period in tenth percent */

void SlowPWM::begin(int pin, unsigned short periodMillis) {
    slowPwmInstance = this;
    timer.end();
    pinMode(pin, OUTPUT);
    this->setPin(HIGH);
    this->pin = pin;
    this->counter = 0;
    this->dc = 0;
    this->nextDc = 0;
    // Call timer handler at 1/1000th the rate specified
    timer.begin(slowPwmCallback, periodMillis, uSec);
}

void SlowPWM::setDutyCycle(double dc) {
    if(dc < 0.0) {
        dc = 0.0;
    } else if(dc > 100.0) {
        dc = 100.0;
    }
    
    this->nextDc = (unsigned short)(dc * 10);
}

void SlowPWM::handler() {
    if(counter == 0) {
        this->dc = this->nextDc;
        if(this->dc != 0) {
            this->setPin(LOW);
        }
    }
    
    if(this->dc <= this->counter) {
        this->setPin(HIGH);
    }
    
    this->counter++;
    if(this->counter == COMPLETE_PERIOD) {
        this->counter = 0;
    }
}

void SlowPWM::setPin(uint8_t value) {
	if (value == LOW)
	{
		PIN_MAP[this->pin].gpio_peripheral->BRR = PIN_MAP[this->pin].gpio_pin;
	} else
	{
		PIN_MAP[this->pin].gpio_peripheral->BSRR = PIN_MAP[this->pin].gpio_pin;
	}
}
