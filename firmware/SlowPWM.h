#ifndef __SLOW_PWM_H__
#define __SLOW_PWM_H__

// Interrupt timer
#include "SparkIntervalTimer/SparkIntervalTimer.h"

class SlowPWM {
    public:
    
    void begin(int pin, unsigned short periodMillis);
    void setDutyCycle(double dc);
    
    // Called internally
    void handler();
    
    private:
    
    static const unsigned short COMPLETE_PERIOD;
    
    int pin;
    unsigned short counter;
    unsigned short dc;
    unsigned short nextDc;
    IntervalTimer timer;
    
    void setPin(uint8_t value);

};

#endif // __SLOW_PWM_H__

