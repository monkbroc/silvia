/*
 * Non-volatile storage for hour meter
 */

#include "storage.h"
#include "helpers.h"
#include "application.h"

const Storage::Data Storage::DEFAULT_DATA = {
  /* version */ 1,
  /* TargetTemperature */ 96.0,
  /* Kp                */ 2.0,
  /* Ki                */ 0.01,
  /* Ko                */ 6.0,
  /* iTermSaturation   */ 10.0,
  /* IntegralErrorBand */ 10.0,
  /* Sleep             */ 0.0,
  /* Twakeup           */ 1436005800.0, /* 6:30am EST */
};

void Storage::read() {
  uint8_t *dataBytes = (uint8_t *)&this->data;
  const unsigned int data_size = sizeof(Storage::Data);

  for(int addr = 0; addr < (int)data_size; addr++) {
    dataBytes[addr] = EEPROM.read(addr);
  }

  this->migrate();
}

void Storage::migrate() {
  const unsigned int data_size = sizeof(Storage::Data);

  while(this->getVersion() != Storage::DEFAULT_DATA.version) {
    switch(this->getVersion()) {
      case 0:
        /* Sleep and Twakeup added */
        this->setSleep(Storage::DEFAULT_DATA.Sleep);
        this->setTwakeup(Storage::DEFAULT_DATA.Twakeup);
        this->setVersion(1);
        break;

      case 1:
        /* Current version */
        break;

      default:
        /* EEPROM was erased */
        memcpy(&this->data, &Storage::DEFAULT_DATA, data_size);
        save(0, data_size);
        break;
    }
  }
}

void Storage::save(int addr, unsigned int length) {
  uint8_t *dataBytes = (uint8_t *)&this->data;

  for(; length > 0; addr++, length--) {
    uint8_t byte = dataBytes[addr];
    EEPROM.write(addr, byte);
  }
}

unsigned int Storage::getVersion() {
  return this->data.version;
}

double Storage::getTargetTemperature() {
  return this->data.TargetTemperature;
}

double Storage::getKp() {
  return this->data.Kp;
}

double Storage::getKi() {
  return this->data.Ki;
}

double Storage::getKo() {
  return this->data.Ko;
}

double Storage::getiTermSaturation() {
  return this->data.iTermSaturation;
}

double Storage::getIntegralErrorBand() {
  return this->data.IntegralErrorBand;
}

double Storage::getSleep() {
  return this->data.Sleep;
}

double Storage::getTwakeup() {
  return this->data.Twakeup;
}

void Storage::setVersion(unsigned int value) {
  if(this->data.version != value) {
    this->data.version = value;
    save(offsetof_and_sizeof(Storage::Data, version));
  }
}

void Storage::setTargetTemperature(double value) {
  if(this->data.TargetTemperature != value) {
    this->data.TargetTemperature = value;
    save(offsetof_and_sizeof(Storage::Data, TargetTemperature));
  }
}

void Storage::setKp(double value) {
  if(this->data.Kp != value) {
    this->data.Kp = value;
    save(offsetof_and_sizeof(Storage::Data, Kp));
  }
}

void Storage::setKi(double value) {
  if(this->data.Ki != value) {
    this->data.Ki = value;
    save(offsetof_and_sizeof(Storage::Data, Ki));
  }
}

void Storage::setKo(double value) {
  if(this->data.Ko != value) {
    this->data.Ko = value;
    save(offsetof_and_sizeof(Storage::Data, Ko));
  }
}

void Storage::setiTermSaturation(double value) {
  if(this->data.iTermSaturation != value) {
    this->data.iTermSaturation = value;
    save(offsetof_and_sizeof(Storage::Data, iTermSaturation));
  }
}

void Storage::setIntegralErrorBand(double value) {
  if(this->data.IntegralErrorBand != value) {
    this->data.IntegralErrorBand = value;
    save(offsetof_and_sizeof(Storage::Data, IntegralErrorBand));
  }
}

void Storage::setSleep(double value) {
  if(this->data.Sleep != value) {
    this->data.Sleep = value;
    save(offsetof_and_sizeof(Storage::Data, Sleep));
  }
}

void Storage::setTwakeup(double value) {
  if(this->data.Twakeup != value) {
    this->data.Twakeup = value;
    save(offsetof_and_sizeof(Storage::Data, Twakeup));
  }
}