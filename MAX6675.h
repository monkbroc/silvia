// this library is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple


#include <application.h>

class MAX6675 {
 public:
  MAX6675(int8_t SCLK_PIN, int8_t CS_PIN, int8_t MISO_PIN);

  double readCelsius(void);
  double readFarenheit(void);
 private:
  static const int16_t pinDelay;

  int8_t sclk, miso, cs;
  byte spiread(void);
};

