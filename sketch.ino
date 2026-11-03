#include <Arduino.h>

#define GPS_RX 16
#define GPS_TX 17

HardwareSerial gpsSerial(2);

void setup() {
  Serial.begin(115200);

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println();
  Serial.println("================================");
  Serial.println("   ESP32 - NEO-6M GPS TEST");
  Serial.println("================================");
  Serial.println("Menunggu data GPS...");
  Serial.println();
}

void loop() {
  while (gpsSerial.available()) {
    char data = gpsSerial.read();
    Serial.write(data);
  }
}