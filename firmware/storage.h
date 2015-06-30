/*
 * Non-volatile storage
 */

#ifndef __STORAGE_H__
#define __STORAGE_H__

class Storage {
  public:
    void read();

    double getTargetTemperature();
    double getKp();
    double getKi();
    double getKo();
    double getiTermSaturation();
    double getIntegralErrorBand();

    void setTargetTemperature(double value);
    void setKp(double value);
    void setKi(double value);
    void setKo(double value);
    void setiTermSaturation(double value);
    void setIntegralErrorBand(double value);

  private:

    void migrate();

    unsigned int getVersion();
    void setVersion(unsigned int value);

    void save(int addr, unsigned int length);

    /* This struct must not be re-ordered since it is the EEPROM layout.
     * Elements must not be deleted.
     * To remove an element, replace the name by _unused1/2/3.
     * Elements must only be added at the end.
     */
    struct Data {
      unsigned int version;

      double TargetTemperature;
      double Kp;
      double Ki;
      double Ko;
      double iTermSaturation;
      double IntegralErrorBand;
    } data;

    static const Data DEFAULT_DATA;
};

#endif // __STORAGE_H__

