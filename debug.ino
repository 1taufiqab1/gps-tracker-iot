#include <Arduino.h>

#define SIM_RX 18
#define SIM_TX 19

HardwareSerial sim800(1);

void sendAT(const char* command) {
  Serial.println();
  Serial.print(">> ");
  Serial.println(command);

  sim800.println(command);

  unsigned long start = millis();

  while (millis() - start < 1500) {
    while (sim800.available()) {
      Serial.write(sim800.read());
    }
  }

  Serial.println();
}

void setup() {
  Serial.begin(115200);
  sim800.begin(9600, SERIAL_8N1, SIM_RX, SIM_TX);

  delay(1500);

  Serial.println("=== SIM800L NETWORK TEST ===");

  sendAT("AT");
  sendAT("AT+CPIN?");
  sendAT("AT+CREG?");
  sendAT("AT+CSQ");

  Serial.println();
  Serial.println("=== TEST SELESAI ===");
}

void loop() {
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}