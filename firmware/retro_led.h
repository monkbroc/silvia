#ifndef __RETRO_LED_H__
#define __RETRO_LED_H__

#include "application.h"
#include "ht16k33.h"

class RetroLED {
    public:
    
    void begin(uint8_t deviceNumber = 0);
    void setBrightness(uint8_t brightness);
    
    void print(float number);
    void setLastDot(bool lastDot);
    
    private:
    
    void refreshDisplay();
    
    static const uint8_t DIGIT_MAPPING[4];
    static const uint8_t NUMBER_CODES[10];
    static const uint8_t DECIMAL_POINT_CODE;
    
    HT16K33 driver;

    uint8_t digits[8];
    boolean lastDot;
};

#endif // __RETRO_LED_H__

