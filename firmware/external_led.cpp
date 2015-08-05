#include "external_led.h"

ExternalLed::ExternalLed(int pin) : pin(pin) {
}

void ExternalLed::begin() {
  pinMode(pin, OUTPUT);
  using namespace std::placeholders;
  RGB.onChange(std::bind(&ExternalLed::handler, this, _1, _2, _3));
}

void ExternalLed::handler(uint8_t r, uint8_t g, uint8_t b) {
  // Mirror green LED to external LED
  analogWrite(pin, g);
}

