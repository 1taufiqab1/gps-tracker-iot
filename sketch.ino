#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#define GPS_RX 16
#define GPS_TX 17

HardwareSerial gpsSerial(2);

char nmeaBuffer[128];
int nmeaIndex = 0;

double nmeaToDecimal(const char* raw, char hemisphere) {
  double value = atof(raw);

  int degrees = (int)(value / 100);
  double minutes = value - (degrees * 100);

  double decimal = degrees + (minutes / 60.0);

  if (hemisphere == 'S' || hemisphere == 'W') {
    decimal = -decimal;
  }

  return decimal;
}

void processGPS(char* sentence) {
  if (strncmp(sentence, "$GPGGA", 6) != 0) {
    return;
  }

  char* fields[15];
  int fieldCount = 0;

  fields[fieldCount++] = sentence;

  for (char* p = sentence; *p != '\0'; p++) {
    if (*p == ',') {
      *p = '\0';

      if (fieldCount < 15) {
        fields[fieldCount++] = p + 1;
      }
    }
  }

  if (fieldCount < 10) {
    Serial.println("Format GPGGA tidak lengkap");
    return;
  }

  int fixQuality = atoi(fields[6]);
  int satellites = atoi(fields[7]);

  Serial.println("\n--- GPS DATA ---");

  if (fixQuality == 0) {
    Serial.println("GPS belum mendapatkan fix.");
    Serial.printf("Satelit: %d\n", satellites);
    return;
  }

  double latitude = nmeaToDecimal(fields[2], fields[3][0]);
  double longitude = nmeaToDecimal(fields[4], fields[5][0]);

  double altitude = atof(fields[9]);

  Serial.println("GPS Fix: VALID");

  Serial.print("Latitude  : ");
  Serial.println(latitude, 6);

  Serial.print("Longitude : ");
  Serial.println(longitude, 6);

  Serial.print("Satellites: ");
  Serial.println(satellites);

  Serial.print("Altitude  : ");
  Serial.print(altitude, 1);
  Serial.println(" m");

  Serial.println("----------------");
}

void setup() {
  Serial.begin(115200);

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println("ESP32 - NEO-6M GPS PARSING");
  Serial.println("Menunggu koordinat GPS...");
}

void loop() {
  while (gpsSerial.available()) {
    char c = gpsSerial.read();

    if (c == '\n') {
      nmeaBuffer[nmeaIndex] = '\0';

      processGPS(nmeaBuffer);

      nmeaIndex = 0;
    }
    else if (c != '\r') {
      if (nmeaIndex < sizeof(nmeaBuffer) - 1) {
        nmeaBuffer[nmeaIndex++] = c;
      } else {
        nmeaIndex = 0;
      }
    }
  }
}