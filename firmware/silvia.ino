// Bubble display
#include "retro_led.h"
#include "ht16k33.h"

// Thermocouple amplifier
#include "MAX6675.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

double temperature = 0;
double relayDc = 0;
String cals;

MAX6675 thermo(SCK, SS, MISO);

const int RELAY_PIN = D3;
const int RELAY_POWER_PIN = D4;
const int RELAY_PERIOD_MILLIS = 1000;

RetroLED display;

void setup() {
    setupSerial();
    setupCloud();
    setupRelay();
    setupDisplay();
}

void setupSerial() {
    Serial.begin(115200);
}

void setupCloud() {
    Spark.connect();
    Spark.variable("temp", &temperature, DOUBLE);
    Spark.variable("dc", &relayDc, DOUBLE);
    Spark.variable("cals", &cals, STRING);
    Spark.function("rssi", rssi);
}

int rssi(String arg) {
    return WiFi.RSSI();
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

const double targetTemperature = 96.0;
const double Kp = 2.0;
const double Ki = 0.01;
const double Koffset = 6.0;
double error = 0;
double pTerm = 0;
double iTerm = 10.0; /* For initial warmup */
const double iTermSaturation = 10; /* Integral anti-windup */
const double integralErrorBand = 10; /* Don't compute integral outside this band */

void controlTemperature() {
    error = targetTemperature - temperature;
    pTerm = Kp * error;
    if(error < integralErrorBand && error > -integralErrorBand) {
        iTerm = limit(Ki * error + iTerm, -iTermSaturation, iTermSaturation);
    }
    relayDc = limit(pTerm + iTerm + Koffset, 0, 100);
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
    
    cals = String("{"
            "\"sp\":" + String(targetTemperature, 1) + ","
            "\"Kp\":" + String(Kp, 1) + ","
            "\"Ki\":" + String(Ki, 3) + ","
            "\"Ko\":" + String(Koffset, 1) + ","
            + "}");
    
}

void controlRelayByDelay() {
    unsigned long start = millis();
    const int period = 1000; /* ms */
    int dc = (int)(relayDc * period / 100.0);
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

