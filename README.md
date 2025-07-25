# 🛰️ GPS Data Extraction and Conversion using Arduino

This project demonstrates how to extract and convert raw GPS data from a `$GPGGA` NMEA sentence using an Arduino and a GPS module.

### ✅ Features

- Reads GPS data from a serial connection  
- Extracts **latitude**, **longitude**, and **time**  
- Converts coordinates from NMEA format (`DDMM.MMMM` / `DDDMM.MMMM`) to **Decimal Degrees**  
- Prints the results to the **Serial Monitor**

---

## 📦 Components Used

- Arduino UNO / Nano  
- GPS Module (e.g., NEO-6M)  
- Jumper Wires  
- USB Cable  
- Optional: Breadboard  

---

## ⚙️ Hardware Setup

| GPS Pin | Arduino Pin |
|---------|-------------|
| VCC     | 5V          |
| GND     | GND         |
| TX      | Pin 2 (RX)  |
| RX      | Pin 3 (TX)  |

> Using `SoftwareSerial` to handle communication on pins **2** and **3**.

---

## 🧪 Sample Raw NMEA Sentence

`$GPGGA,055649.000,0959.6422,N,07617.4754,E,1,08,0.9,545.4,M,46.9,M,,*47`


---

##  Extracted Fields

| Field     | Value       | Meaning                   |
|-----------|-------------|---------------------------|
| Time      | 055649.000  | 05:56:49 UTC              |
| Latitude  | 0959.6422   | 09° 59.6422' North        |
| Longitude | 07617.4754  | 76° 17.4754' East         |
| Fix       | 1           | Valid GPS Fix             |
| Satellites | 08         | 8 satellites used         |

---

## 📐 Latitude & Longitude Conversion

### 🧮 Conversion Formula:

Decimal Degrees = Degrees + (Minutes / 60)


### ▶️ Latitude: `0959.6422`

- Degrees: `09`  
- Minutes: `59.6422`  
- Calculation: `9 + (59.6422 / 60)`  
- **Result:** `9.994037° N`

### ▶️ Longitude: `07617.4754`

- Degrees: `076`  
- Minutes: `17.4754`  
- Calculation: `76 + (17.4754 / 60)`  
- **Result:** `76.291257° E`

---

## 📍 Final Position (Decimal Degrees)

Latitude = 9.994037° N
Longitude = 76.291257° E


🔗 [View on Google Maps](https://www.google.com/maps?q=9.994037,76.291257)

---

## ⏰ Time Conversion

- **Raw Time**: `055649.000`  
- **Parsed**: `05:56:49 UTC`

You can convert UTC to your local timezone manually or programmatically.

---

## 🧑‍💻 Arduino Code Overview

```cpp

#include <SoftwareSerial.h>

// Create a software serial object with RX = pin 2, TX = pin 3
SoftwareSerial m(2, 3); 

void setup() {
  m.begin(9600);           // Start GPS serial communication
  Serial.begin(9600);      // Start Serial Monitor communication
}

// Strings to store raw GPS data
String s, latRaw, lonRaw, utcRaw;

void loop() {
  // Check if data is available from the GPS module
  if (m.available()) {
    if (m.read() == '$') {                 // GPS sentences start with '$'
      s = m.readStringUntil(',');          // Read the sentence type (e.g., GPGGA)

      if (s == "GPGGA") {                  // If sentence is GPGGA (contains fix data)
        utcRaw = m.readStringUntil(',');   // Read UTC time (hhmmss.ss format)
        latRaw = m.readStringUntil(',');   // Read latitude
        m.readStringUntil(',');            // Skip N/S indicator
        lonRaw = m.readStringUntil(',');   // Read longitude
        m.readStringUntil(',');            // Skip E/W indicator

        // Convert raw GPS format to decimal degrees
        float lat = convertToDecimal(latRaw);
        float lon = convertToDecimal(lonRaw);

        // Print latitude and longitude
        Serial.print("Latitude : ");
        Serial.println(lat, 6);
        Serial.print("Longitude : ");
        Serial.println(lon, 6);

        // Print UTC and converted IST time
        printUTCandIST(utcRaw);
      }
    }
  }
}

// Converts GPS coordinate string (ddmm.mmmm) to decimal degrees
float convertToDecimal(String raw) {
  int dotIndex = raw.indexOf('.');         // Find decimal point position
  int degreeLength = (dotIndex == 4) ? 2 : 3; // 2 digits for latitude, 3 for longitude

  int degrees = raw.substring(0, degreeLength).toInt();      // Extract degrees
  float minutes = raw.substring(degreeLength).toFloat();     // Extract minutes

  return degrees + (minutes / 60.0);       // Convert to decimal degrees
}

// Extracts UTC time, converts to IST, and prints both
void printUTCandIST(String utc) {
  // Validate that we have enough characters to extract time
  if (utc.length() < 6) {
    Serial.println("Invalid UTC Time");
    return;
  }

  // Extract hours, minutes, and seconds from hhmmss string
  int hour = utc.substring(0, 2).toInt();
  int minute = utc.substring(2, 4).toInt();
  int second = utc.substring(4, 6).toInt();

  // Print UTC time
  Serial.print("UTC Time: ");
  printTime(hour, minute, second);

  // Convert UTC to IST (UTC + 5 hours 30 minutes)
  minute += 30;
  hour += 5;

  // Handle overflow of minutes
  if (minute >= 60) {
    minute -= 60;
    hour += 1;
  }

  // Handle overflow of hours
  if (hour >= 24) {
    hour -= 24;
  }

  // Print IST time
  Serial.print("IST Time: ");
  printTime(hour, minute, second);
}

// Prints time in hh:mm:ss format with leading zeros
void printTime(int h, int m, int s) {
  if (h < 10) Serial.print('0');
  Serial.print(h); Serial.print(':');

  if (m < 10) Serial.print('0');
  Serial.print(m); Serial.print(':');

  if (s < 10) Serial.print('0');
  Serial.println(s);
}

```

## 📌 OUTPUT

```
Latitude : 12.971598
Longitude : 77.594566
UTC Time: 07:20:15
IST Time: 12:50:15
```
## 📎 Notes

Only $GPGGA is parsed. Extend to handle $GPRMC, $GPGLL, etc.

Negative coordinates (South and West) are not handled in this snippet. You can check direction fields (N/S, E/W) and apply - accordingly.

Time is in UTC; apply your timezone offset for local time.

## 📜 License

This project is open-source.

Feel free to use and modify it for your own projects.

## 🤝 Contributing

Pull requests are welcome!
Want to add:

$GPRMC support?

Timestamp parsing?

Firebase/IoT logging?

Let me know!
