#include "external_led.h"

ExternalLed::ExternalLed(int pin) : pin(pin) {
  pinMode(pin, OUTPUT);
  RGB.onChange(&ExternalLed::handler, this);
}

void ExternalLed::handler(uint8_t r, uint8_t g, uint8_t b) {
  // Mirror green LED to external LED
  analogWrite(pin, g);
}

