// This #include statement was automatically added by the Particle IDE.
#include "cloud_storage.h"

// Non-volatile storage
#include "storage.h"
#include "helpers.h"

// Bubble display
#include "retro_led.h"
#include "ht16k33.h"

// Thermocouple amplifier
#include "MAX6675.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

double temperature = 0;
double relayDc = 0;
char cals[64] = "";

MAX6675 thermo(SCK, SS, MISO);

const int RELAY_PIN = D3;
const int RELAY_POWER_PIN = D4;
const int RELAY_PERIOD_MILLIS = 1000;

RetroLED display;

Storage storage;

void setup() {
    setupStorage();
    setupCloud();
    setupRelay();
    setupDisplay();
}

void setupStorage() {
    storage.read();
}

void setupCloud() {
    Spark.connect();
    Spark.variable("temp", &temperature, DOUBLE);
    Spark.variable("dc", &relayDc, DOUBLE);
    Spark.variable("cals", &cals, STRING);
    Spark.function("rssi", rssi);

    cloudStorage.setup(&storage);    
}

int rssi(String arg) {
    return WiFi.RSSI();
}

int setCal(String arg) {
    return 0;
}

void setupRelay() {
    pinMode(RELAY_POWER_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_POWER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);
}

void setupDisplay() {
    display.begin();
}

void loop() {
    readTemperature();
    controlTemperature();
    publishData();
    updateCals();
    controlRelayByDelay();
}

void readTemperature() {
    temperature = thermo.readCelsius();
    display.print(temperature);
}

inline double limit(double value, double min, double max) {
    if(value < min) {
        return min;
    } else if(value > max) {
        return max;
    } else {
        return value;
    }
}

double error = 0;
double pTerm = 0;
double iTerm = 0;
const double iTermSaturation = 10; /* Integral anti-windup */
const double integralErrorBand = 10; /* Don't compute integral outside this band */

void controlTemperature() {
    error = storage.getTargetTemperature() - temperature;
    pTerm = storage.getKp() * error;
    
    double band = storage.getIntegralErrorBand();
    if(error < band && error > -band) {
        double sat = storage.getiTermSaturation();
        iTerm = limit(storage.getKi() * error + iTerm, -sat, sat);
    }
    relayDc = limit(pTerm + iTerm + storage.getKo(), 0, 100);
}

void publishData() {
    if(Spark.connected()) {
        Spark.publish("coffee", "{"
            "\"temp\":" + String(temperature, 1) + ","
            "\"dc\":" + String(relayDc, 1) + ","
            "\"e\":" + String(error, 1) + ","
            "\"p\":" + String(pTerm, 1) + ","
            "\"i\":" + String(iTerm, 1)
            + "}", 60, PRIVATE);
    }
}

void updateCals() {
    String calsValue = String("{") +
            "\"sp\":" + String(storage.getTargetTemperature(), 1) + ","
            "\"Kp\":" + String(storage.getKp(), 1) + ","
            "\"Ki\":" + String(storage.getKi(), 3) + ","
            "\"Ko\":" + String(storage.getKo(), 1) + ","
            "\"iSat\":" + String(storage.getiTermSaturation(), 1)
            + "}";

  // Set global char array
  calsValue.toCharArray(cals, countof(cals));
}

void controlRelayByDelay() {
    unsigned long start = millis();
    const unsigned int period = 1000; /* ms */
    unsigned int dc = (unsigned int)(relayDc * period / 100.0);
    if(dc > 0) {
        digitalWrite(RELAY_PIN, LOW);
        display.setLastDot(true);
    }
    bool high = false;
    while(millis() - start < period) {
        if(!high && millis() - start >= dc) {
            digitalWrite(RELAY_PIN, HIGH);
            display.setLastDot(false);
            high = true;
        }
        // Process Wi-Fi messages
        SPARK_WLAN_Loop();
        delayMicroseconds(1000); /* 1ms */
    }
}

