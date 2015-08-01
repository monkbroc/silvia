#include "retro_led.h"

/* 7 seg bit positions
   --   33
  |  | 5  4
   --   77
  |  | 2  0
   --   66
*/

const uint8_t RetroLED::DIGIT_MAPPING[4] = { 4, 6, 2, 0 };
const uint8_t RetroLED::NUMBER_CODES[10] = {
    0x7D, // 0
    0x11, // 1
    0xDC, // 2
    0xD9, // 3
    0xB1, // 4
    0xE9, // 5
    0xED, // 6
    0x19, // 7
    0xFD, // 8
    0xF9, // 9
};

const uint8_t RetroLED::DECIMAL_POINT_CODE = 0x02;
const uint8_t RetroLED::DASH_CODE = 0x80;
const uint8_t RetroLED::DODO_CODES[4] = { 0xD5, 0xC5, 0xD5, 0xC5 };

void RetroLED::begin(uint8_t deviceNumber) {
    this->driver.begin(deviceNumber);
}

void RetroLED::print(float number) {
    this->driver.setBrightness();

    // Don't bother with negative numbers right now
    if(number < 0) {
        number = 0;
    } else if(number > 999.9) {
        number = 999.9;
    }
    
    // Print with a single decimal
    // Round correctly
    int wholeNumber = (int)((number * 10) + 0.5);

    // Convert each digit to the correct number code
    for(int i = 3; i >= 0; i--) {
        uint8_t code;
        // Don't show leading zeros
        if(wholeNumber == 0 && i <= 1) {
            code = 0;
        } else {
            code = NUMBER_CODES[wholeNumber % 10];
        }
        this->digits[i] = code;
        wholeNumber /= 10;
    }

    // Add decimal point to the 3rd digit
    this->digits[2] |= DECIMAL_POINT_CODE;
    
    this->refreshDisplay();
}

void RetroLED::printDashes() {
    this->driver.setBrightness();
    for(int i = 0; i < 4; i++) {
        this->digits[i] = DASH_CODE;
    }
    this->refreshDisplay();
}

void RetroLED::printDodo() {
    this->driver.setBrightness(4);
    for(int i = 0; i < 4; i++) {
        this->digits[i] = DODO_CODES[i];
    }
    this->refreshDisplay();
}

void RetroLED::setLastDot(bool lastDot) {
    this->lastDot = lastDot;
    this->refreshDisplay();
}

void RetroLED::refreshDisplay() {
    uint8_t driverDigits[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    
    for(int i = 0; i < 4; i++) {
        driverDigits[DIGIT_MAPPING[i]] = this->digits[i];
    }
    
    // Add last dot if enabled
    driverDigits[DIGIT_MAPPING[3]] |= this->lastDot ? DECIMAL_POINT_CODE : 0;
    
    this->driver.writeDisplay(driverDigits, 0, 8);
}
