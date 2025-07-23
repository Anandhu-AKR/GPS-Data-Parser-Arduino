#include <SoftwareSerial.h>
SoftwareSerial m(2, 3); // RX = 2, TX = 3

void setup() {
  m.begin(9600);
  Serial.begin(9600);
}

String s, latRaw, lonRaw, timeRaw;

void loop() {
  if (m.available()) {
    if (m.read() == '$') {
      s = m.readStringUntil(',');
      
      if (s == "GPGGA") {
        timeRaw = m.readStringUntil(',');     // UTC time
        latRaw = m.readStringUntil(',');      // Latitude
        m.readStringUntil(',');               // N/S
        lonRaw = m.readStringUntil(',');      // Longitude
        m.readStringUntil(',');               // E/W

        // Convert NMEA to decimal degrees
        float lat = convertToDecimal(latRaw);
        float lon = convertToDecimal(lonRaw);

        // Parse and format UTC time
        String hh = timeRaw.substring(0, 2);
        String mm = timeRaw.substring(2, 4);
        String ss = timeRaw.substring(4, 6);

        // Convert to IST (UTC + 5:30)
        int ih = hh.toInt();
        int im = mm.toInt();
        int is = ss.toInt();

        im += 30;
        if (im >= 60) {
          im -= 60;
          ih += 1;
        }

        ih += 5;
        if (ih >= 24) ih -= 24;

        // Print formatted output
        Serial.print("UTC Time: ");
        Serial.print(hh); Serial.print(":");
        Serial.print(mm); Serial.print(":");
        Serial.println(ss);

        Serial.print("IST Time: ");
        if (ih < 10) Serial.print("0");
        Serial.print(ih); Serial.print(":");
        if (im < 10) Serial.print("0");
        Serial.print(im); Serial.print(":");
        if (is < 10) Serial.print("0");
        Serial.println(is);

        Serial.print("Latitude: ");
        Serial.print(lat, 6);
        Serial.print("  Longitude: ");
        Serial.println(lon, 6);
      }
    }
  }
}

float convertToDecimal(String raw) {
  if (raw.length() < 6) return 0.0;

  int dotIndex = raw.indexOf('.');
  int degreeLength = dotIndex == 4 ? 2 : 3;

  int degrees = raw.substring(0, degreeLength).toInt();
  float minutes = raw.substring(degreeLength).toFloat();

  return degrees + (minutes / 60.0);
}
