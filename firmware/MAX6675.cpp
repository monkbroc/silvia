// this library is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple


#include "MAX6675.h"

const int16_t MAX6675::pinDelay = 10;

MAX6675::MAX6675(int8_t SCLK_PIN, int8_t CS_PIN, int8_t MISO_PIN) {
  sclk = SCLK_PIN;
  cs = CS_PIN;
  miso = MISO_PIN;

  //define pin modes
  pinMode(cs, OUTPUT);
  pinMode(sclk, OUTPUT); 
  pinMode(miso, INPUT);

  digitalWrite(cs, HIGH);
}
double MAX6675::readCelsius(void) {

  uint16_t v;

  digitalWrite(cs, LOW);
  delayMicroseconds(pinDelay);

  v = spiread();
  v <<= 8;
  v |= spiread();

  digitalWrite(cs, HIGH);

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return 0; 
    //return -100;
  }

  v >>= 3;

  return v*0.25;
}

double MAX6675::readFarenheit(void) {
  return readCelsius() * 9.0/5.0 + 32;
}

byte MAX6675::spiread(void) { 
  int i;
  byte d = 0;

  for (i=7; i>=0; i--)
  {
    digitalWrite(sclk, LOW);
    delayMicroseconds(pinDelay);
    if (digitalRead(miso)) {
      //set the bit to 0 no matter what
      d |= (1 << i);
    }

    digitalWrite(sclk, HIGH);
    delayMicroseconds(pinDelay);
  }

  return d;
}
