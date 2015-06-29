// Thermocouple amplifier
#include "MAX6675.h"

double temperature = 0;
double relayDc = 0;

MAX6675 thermo(SCK, SS, MISO);

const int RELAY_PIN = D0;
const int RELAY_POWER_PIN = D1;
const int RELAY_PERIOD_MILLIS = 1000;

void setup() {
    setupSerial();
    setupCloud();
    setupRelay();
}

void setupSerial() {
    Serial.begin(115200);
}

void setupCloud() {
    Spark.variable("temp", &temperature, DOUBLE);
    Spark.variable("dc", &relayDc, DOUBLE);
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

void loop() {
    readTemperature();
    controlTemperature();
    publishData();
    controlRelayByDelay();
}

void readTemperature() {
    temperature = thermo.readCelsius();
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
const double Koffset = 9.0;
double pTerm = 0;
double iTerm = 4.0; /* For initial warmup */
const double iTermLim = 10;

void controlTemperature() {
    double error = targetTemperature - temperature;
    pTerm = Kp * error;
    if(error < 10 && error > -10) {
        iTerm = limit(Ki * error + iTerm, -iTermLim, iTermLim);
    }
    relayDc = limit(pTerm + iTerm + Koffset, 0, 100);
}

void publishData() {
    Spark.publish("coffee", "{"
        "\"temp\":" + String(temperature) + ","
        "\"dc\":" + String(relayDc) + ","
        "\"p\":" + String(pTerm) + ","
        "\"i\":" + String(iTerm)
        + "}", 60, PRIVATE);
}

void controlRelayByDelay() {
    unsigned long start = millis();
    const int period = 1000; /* ms */
    int dc = (int)(relayDc * period / 100.0);
    if(dc > 0) {
        digitalWrite(RELAY_PIN, LOW);
    }
    bool high = false;
    while(millis() - start < period) {
        if(!high && millis() - start >= dc) {
            digitalWrite(RELAY_PIN, HIGH);
            high = true;
        }
        // Process Wi-Fi messages
        SPARK_WLAN_Loop();
        delayMicroseconds(1000); /* 1ms */
    }
}

