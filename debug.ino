#include <Arduino.h>

#define SIM_RX 18
#define SIM_TX 19

HardwareSerial sim800(1);

unsigned long lastAT = 0;

void setup() {
  Serial.begin(115200);

  sim800.begin(9600, SERIAL_8N1, SIM_RX, SIM_TX);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" ESP32 - SIM800L AT TEST");
  Serial.println("==============================");
  Serial.println("Menunggu respons modem...");
}

void loop() {
  while (sim800.available()) {
    Serial.write(sim800.read());
  }

  if (millis() - lastAT >= 2000) {
    sim800.println("AT");
    lastAT = millis();
  }
}