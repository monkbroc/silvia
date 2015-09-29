#ifndef __EXTERNAL_LED_H__
#define __EXTERNAL_LED_H__

#include "application.h"

class ExternalLed {
  public:
    ExternalLed(int pin);
    void handler(uint8_t r, uint8_t g, uint8_t b);

  private:
    int pin;
};

#endif // __EXTERNAL_LED_H__
